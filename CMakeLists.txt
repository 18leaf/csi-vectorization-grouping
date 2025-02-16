cmake_minimum_required(VERSION 3.29)
project(csi_vectorization_grouping)

set(CMAKE_CXX_STANDARD 20)

add_executable(csi_vectorization_grouping main.cpp
        include/ipc_channel.h
        include/message.h
        include/message_queue.h
        include/concurrent_hashmap.h
        include/logger.h
        include/vectorizer.h
        src/ipc_channel.cpp
        src/vectorizer.cpp
        include/IPCManager.hpp
        include/WorkerPool.hpp
        include/UIUpdater.hpp
)
