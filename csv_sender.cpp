//
// Created by Lindsey on 2/20/2025.
//
// csv_sender.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <chrono>
#include <thread>
#include <cerrno>
#include <cstring>

int main() {
	const std::string csvFile = "data/synthetic_ipc.csv";
	const std::string pipePath = "/tmp/ipc_pipe";

	// create named pipe first in first out
	if (mkfifo(pipePath.c_str(), 0666) < 0) {
		if (errno != EEXIST) {
			std::cerr << "Error creating FIFO " << pipePath << ": " << std::strerror(errno) << std::endl;
			return 1;
		}
	}

	// open csv file
	std::ifstream csv(csvFile);
	if (!csv.is_open()) {
		std::cerr << "Failed to open CSV file: " << csvFile << std::endl;
		return 1;
	}

	// open FIFO for writing, is blocked until reader opens it
	int pipeFd = open(pipePath.c_str(), O_WRONLY);
	if (pipeFd < 0) {
		std::cerr << "Failed to open FIFO for writing: " << std::strerror(errno) << std::endl;
		return 1;
	}

	std::string line;
	while (std::getline(csv, line)) {
		// check for newline at end, append if not there
		if (line.back() != '\n') {
			line.push_back('\n');
		}
		ssize_t written = write(pipeFd, line.c_str(), line.size());
		if (written < 0) {
			std::cerr << "Error writing to FIFO: " << std::strerror(errno) << std::endl;
			break;
		}
		// simulate real time delay between messages
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	close(pipeFd);
	return 0;
}
