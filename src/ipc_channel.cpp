//
// Created by Lindsey on 2/15/2025.
//

#include "../include/ipc_channel.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

class SyntheticIPCChannel : public IPCChannel {
public:
	explicit SyntheticIPCChannel(const std::string& filepath) : file_stream(filepath) {
		if (!file_stream.is_open()) {
			throw std::runtime_error("Failed to open synthetic IPC file " + filepath);
		}
	}

	std::string read() override {
		// parse line by lime (csv format)
		if (std::string line; std::getline(file_stream, line)) {
			return line;
		};
		return "";
	}

private:
	std::ifstream file_stream;
};


IPCChannel* createSyntheticIPCChannel(const std::string& filepath) {
	return new SyntheticIPCChannel(filepath);
}