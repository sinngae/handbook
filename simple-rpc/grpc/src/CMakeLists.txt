cmake_minimum_required(VERSION 2.8.7)

project(hiwork C CXX)
get_filename_component(CMakeFilePath "./CMakeLists.txt" ABSOLUTE)
get_filename_component(RootDir "${CMakeFilePath}" PATH)
#message("CMakeFilePath:${CMakeFilePath}\nrootdir:${RootDir}")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -Os")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -Os")
set(CMAKE_CXX_FLAGS "-Wno-error=format-security -DONECLOUD -D_FILE_OFFSET_BITS=64 -fPIC -pipe -I${RootDir} -std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-L/home/local/lib ${CMAKE_EXE_LINKER_FLAGS}")

include_directories(/home/local/include)
include_directories(uniform_ecode)
include_directories(log)

#lib grpc
find_library(PROTOBUF_LIB NAMES libprotobuf.a HINTS /home/local/lib)
find_library(GRPC++_LIB NAMES libgrpc++.a HINTS /home/local/lib)
find_library(GRPC_LIB NAMES libgrpc.a HINTS /home/local/lib)
find_library(GRPC++_REFLECTION_LIB NAMES libgrpc++_reflection.a HINTS /home/local/lib)

#grpc program
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
find_program(_PROTOBUF_PROTOC protoc)
  
# Proto file
get_filename_component(hiwork_proto "../protos/hiwork.proto" ABSOLUTE)
get_filename_component(hiwork_proto_path "${hiwork_proto}" PATH)

# Generated sources
set(hiwork_proto_srcs "hiwork.pb.cc")
set(hiwork_proto_hdrs "hiwork.pb.h")
set(hiwork_grpc_srcs "hiwork.grpc.pb.cc")
set(hiwork_grpc_hdrs "hiwork.grpc.pb.h")
add_custom_command(
      OUTPUT "${hiwork_proto_srcs}" "${hiwork_proto_hdrs}" "${hiwork_grpc_srcs}" "${hiwork_grpc_hdrs}" 
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hiwork_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hiwork_proto}"
      DEPENDS "${hiwork_proto}")

include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}"
	)


file(GLOB SOURCES *.cc)
file(GLOB UNIECODE_SOURCES ./uniform_ecode/*.c)
add_executable(${PROJECT_NAME} ${SOURCES} ${hiwork_proto_srcs} ${hiwork_grpc_srcs} ${UNIECODE_SOURCES})
target_link_libraries(${PROJECT_NAME} 
	json json-c
	pthread dl  m z crypto ssl pcre zlog
	${PROTOBUF_LIB} ${GRPC++_LIB} ${GRPC_LIB} ${GRPC++_REFLECTION_LIB})
#glib-2.0 stdc++
