#INCLUDE_DIRECTORIES([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
#CMAKE_INCLUDE_PATH 和 CMAKE_LIBRARY_PATH这两个是环境变量
#INCLUDE_DIRECTORIES(/usr/local/include/hiwork)
#更好地用
#FIND_PATH (myHeader NAMES hello.h PATHS /usr/local/include /usr/local/include/hello)
FIND_PATH(myheader hiwork.h PATHS /usr/local/include/hiwork)
IF(myheader)
	INCLUDE_DIRECTORIES(${myheader})
ENDIF(myheader)


FIND_LIBRARY(mylib hiwork PATHS /usr/local/lib)
IF(mylib)
	#LINK_LIBRARIES("/usr/local/lib/libhiwork.so")
	LINK_LIBRARIES(${mylib})
ENDIF(mylib)

#
ADD_DEFINITIONS(
	-O3 -g -W -Wall
	-Wunused-variable -Wunused-parameter -Wunused-function -Wunused
	-Wno-deprecated -Woverloaded-virtual -Wwrite-strings
	-D__WUR= -D__REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL)

#
ADD_EXECUTABLE(main main.cpp)

#
TARGET_LINK_LIBRARIES(main ${mylib})
