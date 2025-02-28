cmake_minimum_required(VERSION 3.10)
project(ls)
set(CMAKE_CXX_STANDARD 17)
set(SRC_FILES
    src/ls_command.cpp
    src/logger.cpp
)

set(HEADER_FILES
    inc/ls_command.h
    inc/logger.h
    inc/object.h
)

add_executable(ls main.cpp ${SRC_FILES} ${HEADER_FILES})

target_include_directories(ls PRIVATE inc)
target_compile_options(ls PRIVATE -Wall)
