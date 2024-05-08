#����Դ����,��ʹ��add_executable��add_library, һ����ʾ�����ִ���ļ���һ����ʾ������ļ�
#ָ��ͷ�ļ�������·������ʹ��include_directories
#ָ�����ӿ��Ѱ��Ŀ¼������ʹ��link_directories
#����ĳ�������⣬��ʹ��target_link_libraries
#����꣬��ʹ��add_definitions

#��ʾҪ�����Сcamke�汾��
cmake_minimum_required(VERSION 3.20)


#����һ������ PROJECT_NAME
set(PROJECT_NAME "BuriedPoint")

#cmake��Ŀ��Ŀ¼��CMakeLists.txt������Ҫ�����
project(${PROJECT_NAME} VERSION 1.1.1.1)

#���忪�أ��Ƿ���뾲̬��
option(BUILD_BURIED_SHARED_LIBS "build shared library" ON)
#���忪�أ��Ƿ���붯̬��
option(BUILD_BURIED_STATIC_LIBS "build static library" ON)
#���忪�أ���ʾ�Ƿ����ʾ������
option(BUILD_BURIED_EXAMPLES "build examples" OFF)
#���忪�أ���ʾ�Ƿ���뵥Ԫ���Գ���
option(BUILD_BURIED_TESTS "build unittest" OFF)
#���忪�أ���ʾ�����Ƿ�ʹ�ö��߳�
option(BUILD_BURIED_FOR_MT "build for /MT" OFF)

#MSVC ����ר�õ����ã���ʾ�������̱߳��룬�����Linux�£������Ҫע�͵�
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} /MP")

#���Ҫʹ�ö��̣߳�����DEBUG��RELEASE���ֱ���ģʽʱ���������߳�ģʽ������
if(BUILD_BURIED_FOR_MT)
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

#�൱��libs = libs + winmm + iphlpapi + ws2_32 +dbghelp
set(LIBS ${LIBS} winmm iphlpapi ws2_32 dbghelp)

#����pdb�ļ�,�����ļ���¼�˱���δ֪��������ַ�ȣ�Ψ�д��ڸ��ļ��ſɽ���debug
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}	/Zi")  #����C++���������ɵ�����Ϣ
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}		/Zi")	#C���������ɵ�����Ϣ
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:RED /OPT:ICF")

set(INCDIRS . lib ${CMAKE_BINARY_DIR})
include_directories(${INCDIRS})

#��ʾ����C++20��C11��׼
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std:c11")
#[[
	��ʹ����һ�ַ�ʽ��ָ����Ŀ��������C++��׼����
	set(CMAKE_CXX_STANDARD_REQUIRED ON))	#����C++��׼���ù��ܣ����������ñ�׼Ϊ����Ҫ��������������ѡ�ð汾����������ʹ����һ�汾
	set(CMAKE_CXX_STANDARD 11)#����ñ�׼c++11
]]

#��ӡ��Ϣ
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")

#Debugģʽ�¶���DEBUG�꣬ Releaseģʽ�¶���NDEBUG��
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DDEBUG)
else()
	add_definitions(-DNDEBUG)
endif()

#����src��Ŀ¼����Ŀ�У���Ŀ¼��Ҳ���Ӧ��һ��CMakeLists
add_subdirectory(src)
#�Ƿ����example��Ŀ¼���ǵĻ�����ô�������ӵ���Ŀ�У�������Ŀ¼��Ӧ��Ҳ�ж�Ӧ��һ��CMakeLists
if(BUILD_BURIED_EXAMPLES)
	add_subdirectory(examples)
endif()
#��example��Ŀ¼���ƣ�������Ҫ���������Թ��߿⣬��Ҫ���ӽ�ȥ
if(BUILD_BURIED_TEST)
	include_directories(
		googletest/googletest
		googletest/googletest/include
	)
	add_subdirectory(googletest)
	add_subdirectory(tests)
endif()
