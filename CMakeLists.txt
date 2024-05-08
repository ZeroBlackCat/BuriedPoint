#编译源程序,可使用add_executable或add_library, 一个表示编译可执行文件，一个表示编译库文件
#指定头文件的搜索路径，可使用include_directories
#指定链接库的寻找目录，可以使用link_directories
#连接某个三方库，可使用target_link_libraries
#定义宏，可使用add_definitions

#表示要求的最小camke版本号
cmake_minimum_required(VERSION 3.20)


#定义一个变量 PROJECT_NAME
set(PROJECT_NAME "BuriedPoint")

#cmake项目根目录的CMakeLists.txt都必须要有这个
project(${PROJECT_NAME} VERSION 1.1.1.1)

#定义开关，是否编译静态库
option(BUILD_BURIED_SHARED_LIBS "build shared library" ON)
#定义开关，是否编译动态库
option(BUILD_BURIED_STATIC_LIBS "build static library" ON)
#定义开关，表示是否编译示例代码
option(BUILD_BURIED_EXAMPLES "build examples" OFF)
#定义开关，表示是否编译单元测试程序
option(BUILD_BURIED_TESTS "build unittest" OFF)
#定义开关，表示编译是否使用多线程
option(BUILD_BURIED_FOR_MT "build for /MT" OFF)

#MSVC 编译专用的配置，表示启动多线程编译，如果是Linux下，这个需要注释掉
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} /MP")

#如果要使用多线程，则在DEBUG和RELEASE两种编译模式时，开启多线程模式来编译
if(BUILD_BURIED_FOR_MT)
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

#相当于libs = libs + winmm + iphlpapi + ws2_32 +dbghelp
set(LIBS ${LIBS} winmm iphlpapi ws2_32 dbghelp)

#生成pdb文件,该类文件记录了变量未知，函数地址等，唯有存在该文件才可进行debug
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}	/Zi")  #告诉C++编译器生成调试信息
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}		/Zi")	#C编译器生成调试信息
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:RED /OPT:ICF")

set(INCDIRS . lib ${CMAKE_BINARY_DIR})
include_directories(${INCDIRS})

#表示启用C++20和C11标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std:c11")
#[[
	可使用另一种方式来指定项目编译器的C++标准设置
	set(CMAKE_CXX_STANDARD_REQUIRED ON))	#开启C++标准设置功能，后续的设置标准为必须要求，若无设置则优选该版本，不可用则使用上一版本
	set(CMAKE_CXX_STANDARD 11)#如采用标准c++11
]]

#打印信息
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")

#Debug模式下定义DEBUG宏， Release模式下定义NDEBUG宏
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DDEBUG)
else()
	add_definitions(-DNDEBUG)
endif()

#添加src子目录到项目中，子目录中也会对应有一个CMakeLists
add_subdirectory(src)
#是否编译example子目录，是的话，那么将其添加到项目中，并且子目录中应该也有对应的一个CMakeLists
if(BUILD_BURIED_EXAMPLES)
	add_subdirectory(examples)
endif()
#与example子目录类似，由于需要第三方测试工具库，需要添加进去
if(BUILD_BURIED_TEST)
	include_directories(
		googletest/googletest
		googletest/googletest/include
	)
	add_subdirectory(googletest)
	add_subdirectory(tests)
endif()

