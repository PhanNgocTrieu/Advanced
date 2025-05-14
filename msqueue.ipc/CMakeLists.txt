cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MsgQueueProject)

# Add the library
add_library(msg_queue SHARED lib/msg_queue.cpp)

# Include directories
target_include_directories(msg_queue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# set bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Add the executables
add_executable(read read.cpp)
add_executable(write write.cpp)

# Link the library to the executables
target_link_libraries(read msg_queue)
target_link_libraries(write msg_queue)

# Include directories for executables
target_include_directories(read PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(write PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)