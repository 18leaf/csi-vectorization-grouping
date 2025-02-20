//
// Created by Lindsey on 2/15/2025.
//

#include "ipc_channel.h"
#include <stdexcept>
#include <string>

#ifdef _WIN32

// SyntheticIPCChannel for Windows
#include <fstream>

class SyntheticIPCChannel : public IPCChannel {
public:
    explicit SyntheticIPCChannel(const std::string& filepath)
        : file_stream(filepath) {
        if (!file_stream.is_open()) {
            throw std::runtime_error("Failed to open synthetic IPC file: " + filepath);
        }
    }

    std::string read() override {
        std::string line;
        if (std::getline(file_stream, line)) {
            return line;
        }
        return "";
    }

private:
    std::ifstream file_stream;
};

IPCChannel* createSyntheticIPCChannel(const std::string& filepath) {
    return new SyntheticIPCChannel(filepath);
}

#else

// linux using named pipe

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

class RealIPCChannel : public IPCChannel {
public:
    explicit RealIPCChannel(const std::string& pipePath)
        : pipePath_(pipePath) {
        // create name pipe if not exists
        if (mkfifo(pipePath_.c_str(), 0666) < 0) {
            if (errno != EEXIST) {
                throw std::runtime_error("Failed to create FIFO " + pipePath_ + ": " + std::strerror(errno));
            }
        }
        // open read only
        fd_ = open(pipePath_.c_str(), O_RDONLY | O_NONBLOCK);
        if (fd_ < 0) {
            throw std::runtime_error("Failed to open FIFO " + pipePath_ + " for reading: " + std::strerror(errno));
        }
    }

    ~RealIPCChannel() override {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

    std::string read() override {
        char buffer[256];
        ssize_t bytesRead = ::read(fd_, buffer, sizeof(buffer) - 1);
        if (bytesRead < 0) {
            if (errno == EAGAIN) {
                // no data
                return "";
            }
            throw std::runtime_error("Error reading from FIFO: " + std::string(std::strerror(errno)));
        }
        if (bytesRead == 0) {
            // eof and no data
            return "";
        }
        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }

private:
    std::string pipePath_;
    int fd_;
};

IPCChannel* createRealIPCChannel(const std::string& pipePath) {
    return new RealIPCChannel(pipePath);
}

#endif
