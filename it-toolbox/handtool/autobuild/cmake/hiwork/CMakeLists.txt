#建立工程
PROJECT(HIWORK)

#指定源代码地址
AUX_SOURCE_DIRECTORY(. DIR_SRC)

FILE(GLOB HEADERS "*.h")

#指定编译输出位置 此为build/lib
#ADD_SUBDIRECTORY(lib lib_new) 可变更为build/lib_new
ADD_SUBDIRECTORY(lib)

SET(LIBHIWORK_SRC hiwork.cpp)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#三种库：1.shared .so文件 2.static .a文件 3.module 第三种仅当支持dyld时有效，否则同shared
ADD_LIBRARY(hiwork SHARED ${DIR_SRC})

#hiwork在上面已经是个target，不能重名，所以写为hiwork_static
ADD_LIBRARY(hiwork_static STATIC ${DIR_SRC})
#期望以hello为名输出
SET_TARGET_PROPERTIES(hiwork_static PROPERTIES OUTPUT_NAME "hello")
GET_TARGET_PROPERTY(OUTPUT_VALUE hello_static OUTPUT_NAME)
MESSAGE(STATUS "This is the hiwork_static OUTPUT_NAME:"${OUTPUT_VALUE})

#清理掉其他使用这个target名字的库
SET_TARGET_PROPERTIES(hiwork PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(hiwork_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

#按规则，给动态库指定版本号 VERSION指代动态库版本，SOVERSION指代API版本
SET_TARGET_PROPERTIES(hiwork PROPERTIES VERSION 1.2 SOVERSION 1)

#安装
INSTALL(TARGETS hiwork hiwork_static 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
INSTALL(FILES ${HEADERS}
	DESTINATION include/hiwork)
