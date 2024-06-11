#在使用add_executable之前可以预先赋值给特定宏来设置可执行程序的存放路径
#[[
	宏：EXECUTABLE_OUTPUT_PATH
	如：
	set(HOME /home/robin/Linux/Sort)
	set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)
]]
#[[
	若存在联合多个源文件编译，则可以使用aux_source_directory(<dir> <valuable>)
	如：
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
	add_executable(app  ${SRC_LIST})
	其中，CMAKE_CURRENT_SOURCE_DIR为当前CMakeLists所在的路径
	此外，还可用另一种方式来进行编译
	file(GLOB/GLOB_RECURSE 变量名 要搜索的文件路径和文件类型)
	例如：
	file(GLOB MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) 或file(GLOB MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp") 
	file(GLOB MAIN_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h) 或 同上表述
]]
cmake_minimum_required(VERSION 3.20)
project(BuriedExample)

add_definitions(-D_WIN32_WINNT=0x0601)
include_directories(. .. ../src ../src/context ../src/third_package ../src/third_package/spdlog/include ../src/third_package/boost)

add_executable(buried_example buried_example.cc)
target_link_libraries(buried_example Buried_static)

add_executable(context_example context_example.cc)
target_link_libraries(context_example Buried_static)
