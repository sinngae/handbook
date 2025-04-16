cmake_minimum_required(VERSION 2.8.7)

project(hitest)

set(CMAKE_C_FLAGS "-Wall -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -I/opt/local/include ${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-L/opt/local/lib ${CMAKE_EXE_LINKER_FLAGS}")

aux_source_directory(. SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
	pthread ssl crypto json-c zlog z curl pthread)