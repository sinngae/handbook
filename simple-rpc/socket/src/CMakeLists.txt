cmake_minimum_required(VERSION 2.8)
#
PROJECT(mini-rpc)
#ADD_SUBDIRECTORY(src)

INCLUDE_DIRECTORIES(/home/local/include)
LINK_DIRECTORIES(/home/local/lib)
INCLUDE_DIRECTORIES(./log)
INCLUDE_DIRECTORIES(./uniform_ecode)
INCLUDE_DIRECTORIES(./thread_pool)
INCLUDE_DIRECTORIES(./mini_rpc)
INCLUDE_DIRECTORIES(./servlet)

#AUX_SOURCE_DIRECTORY(. DIR_SRC)
FILE(GLOB ECODE_SRCS "./uniform_ecode/*.c")
FILE(GLOB THPOOL_SRCS "./thread_pool/*.c")
FILE(GLOB RPC_SRCS "./mini_rpc/*.c")
FILE(GLOB SERVLET_SRCS "./servlet/*.c")
FILE(GLOB HDRS "*.h")
FILE(GLOB SRCS "*.c")

#
ADD_DEFINITIONS(
	-O3 -g -W -Wall
	-Wunused-variable -Wunused-parameter -Wunused-function -Wunused
	-Wno-deprecated -Wwrite-strings
	-D__WUR= -D__REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL)
#-Woverloaded-virtual


ADD_EXECUTABLE(${PROJECT_NAME} ${ECODE_SRCS} ${THPOOL_SRCS} ${RPC_SRCS} ${SERVLET_SRCS} ${SRCS})
#ADD_LIBRARY(hiwork SHARED ${DIR_SRC})
#ADD_LIBRARY(hiwork_static STATIC ${DIR_SRC})


#SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION 1.2 SOVERSION 1)
#
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ev pthread zlog json)

#安装
#INSTALL(TARGETS hiwork hiwork_static 
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib)
#INSTALL(FILES ${HEADERS}
#	DESTINATION include/hiwork)
