cmake_minimum_required(VERSION 3.10)
project(csi-vectorization-grouping)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)


set(RECEIVER_SOURCES
        main.cpp
        src/ipc_channel.cpp
        src/vectorizer.cpp
)

add_executable(receiver ${RECEIVER_SOURCES})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# sender source for linux
add_executable(sender csv_sender.cpp)

# copy data to runtime for testing
add_custom_command(TARGET sender POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
)
