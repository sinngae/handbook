cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

project(hiwork LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get build timestamp
string(TIMESTAMP BUILD_TIMESTAMP UTC)


include_directories(
	"${PROJECT_SOURCE_DIR}/include"
	)

link_directories(
	${PROJECT_BINARY_DIR}/lib
	)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)

message(STATUS "System name       : ${CMAKE_SYSTEM_NAME}")
message(STATUS "Project Name      : ${PROJECT_NAME}")
message(STATUS "Project directory : ${PROJECT_SOURCE_DIR}")
message(STATUS "Build called from : ${PROJECT_BINARY_DIR}")
message(STATUS "Executables in    : ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "Libraries in      : ${LIBRARY_OUTPUT_DIRECTORY}")

