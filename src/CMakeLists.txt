#由于src存放的源文件最终是用于生成
include_guard(GLOBAL)

#添加第三方库
include_directories(. third_package third_package/spdlog/include third_package/boost third_package/mbedtls/include)
#在使用spdlog时，若要自定义信息格式，则需要事先定义宏
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
add_definitions(-DBOOST_JSON_NO_LIB)
add_definitions(-D_WIN32_WINNT=0x0601)
add_subdirectory(third_package/spdlog)
add_subdirectory(third_package/mbedtls/library)

set(DB_SRCS third_package/sqlite/sqlite3.c database/database.cc)


set(BURIED_SRCS
	${DB_SRCS}
	crypt/crypt.cc
	report/http_report.cc
	report/buried_report.cc
	common/common_service.cc
	context/context.cc
	buried.cc
	buried_core.cc
)

set(MODULE_NAME "Buried")
message(STATUS "libs ${LIBS}")


#编译动态库
#[[
	在静态库中
]]
if(BUILD_BURIED_SHARED_LIBS)
	add_library(${MODULE_NAME} SHARED ${BURIED_SRCS}) #类似add_executable，不过最终生成的是库文件libBuried.a/libBuried.dll，此处为动态库文件
	set_target_properties(${MODULE_NAME} PROPERTIES COMPILE_FLAGS "/EHsc")
	target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS}) #该动态库需要依赖LIBS的库文件
endif()

#编译静态库
if(BUILD_BURIED_STATIC_LIBS)
	add_library(${MODULE_NAME}_static STATIC ${BURIED_SRCS})#类似add_executable，不过最终生成的是库文件libBuried.so/libBuried.lib，此处为静态库文件
	set_target_properties(${MODULE_NAME} PROPERTIES COMPILE_FLAGS "/EHsc")
	target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS})
endif()