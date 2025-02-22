# CS 3502 Project 1 – Multi-Threaded Programming and IPC - Live Message Vector Embeddings

This project demonstrates the concepts of multi-threading, synchronization, and interprocess communication (IPC) using C++ on both Linux and Windows. The implementation includes a multi-threaded application that processes messages (simulated from a CSV file) using a shared MessageQueue, a concurrent hash map for storing vector embeddings, and a simple command-line UI updater. Additionally, a separate sender process simulates real-time message generation via a named pipe (FIFO) on Linux.

---

## Overview

This project is part of the CS 3502 course on multi-threaded programming and IPC. It is designed to demonstrate:
- **Multi-threading:** Creating and managing threads, ensuring thread safety with mutexes, condition variables, and shared_mutex.
- **Synchronization and Resource Protection:** Using proper locking mechanisms to avoid race conditions.
- **Deadlock Creation and Prevention:** A demo module shows how deadlocks can occur and how to resolve them.
- **Interprocess Communication (IPC):** Communication between processes using pipes. On Linux, the project uses a named pipe (FIFO) for IPC.
- **Command-Line UI:** A simple UI updater prints a summary of the processed messages and their fake vector embeddings.

## Features

- **Multi-threading Implementation:**
    - An `IPCManager` thread reads messages from an IPC channel and enqueues them.
    - A `WorkerPool` with multiple threads dequeues messages, generates fake vector embeddings, and stores them in a concurrent hash map.
    - A `UIUpdater` thread periodically displays the newest messages and their embeddings.
- **Inter-Process Communication:**
    - On Linux, the receiver reads messages from a named pipe (`/tmp/ipc_pipe`).
    - A separate sender process (`csv_sender`) reads messages from a CSV file and writes them to the pipe.
    - On Windows, a synthetic IPC channel reads from a CSV file for testing.
- **Deadlock Demonstration:**
    - An extras module demonstrates both a deadlock scenario and its resolution using proper lock ordering.

## Project Structure

    project-root/ 
    ├── CMakeLists.txt
    ├── README.md
    │
    ├── main.cpp                              # 
    ├── csv_sender.cpp                        # (Linux) reads from the synthetic data file and sends messages to the pipe with delays
    │
    ├── include/ 
    │   ├── concurrent_hashmap.h
    │   ├── IPCManager.hpp                    # Managing ipc - recieves messaged then adds to message_qeueue
    │   ├── ipc_channel.h
    │   ├── logger.h                          # 
    │   ├── message.h
    │   ├── message_queue.h                   # Thread safe queue for messages ready to be processed
    │   ├── UIUpdater.hpp                     # Update UI - displays n recent VectorObjects + msg content from concurrent hashmap
    │   ├── vectorizer.h
    │   └── WorkerPool.hpp                    # Thread Pool -> Retrieve from message_queue, "vectorize", add to concurrent hashmap
    │
    ├── src/ 
    │   └── ipc_channel.cpp                   # (Linux) establishes ipc connection (Windows) opens data csv file
    │   └── vectorizer.cpp                    # placeholder for actual implementation of a vectorizer (outside of OS assignment scope)
    │
    ├── data/synthetic_ipc.csv
    │
    └── extras/message_queue_deadlock_demo.cpp
    

___
## Installation

### Prerequisites

- **C++ Compiler:** GCC or Clang
- **CMake:** Version 3.10 or higher

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/18leaf/live-message-vectorization
   cd live-message-vectorization

2. Make Build directory and run CMake
   ```bash
    mkdir build
    cd build
    cmake ..
    make
3. Run main program (receiver) and sender (if using Linux)
   - if using linux you need to start the sender process as well
   ```bash
    cd build/bin
   ./sender 
   ```
   - Run main program
   ```bash
   cd build/bin
   ./receiver
   ```