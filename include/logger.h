#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <string>

class Logger {
public:
	static void log(const std::string& message) {
		// lock mutex so only 1 thread prints at a time.
		std::lock_guard<std::mutex> guard(getMutex());

		const auto now = std::chrono::system_clock::now();
		const std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::cout << "[" << std::to_string(now_c) << "] " << message << std::endl;
	}

private:
	// Return a reference to a static mutex that is used for logging.
	static std::mutex& getMutex() {
		static std::mutex mtx;
		return mtx;
	}
};

#endif // LOGGER_H
