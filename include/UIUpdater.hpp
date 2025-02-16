//
// Created by Lindsey on 2/15/2025.
//

#ifndef UIUPDATER_HPP
#define UIUPDATER_HPP
#include <atomic>
#include <chrono>
#include <thread>

#include "concurrent_hashmap.h"
#include "logger.h"

class UIUpdater {
public:
	UIUpdater(ConcurrentHashMap& buckets, const std::chrono::milliseconds interval)
		: groupBuckets(buckets), updateInterval(interval), isRunning(true) {}

	~UIUpdater() {
		stop();
	}

	void start() {
		// new thread for ui
		// TODO load state from last time??
		uiThread = std::thread(&UIUpdater::run, this);
	}

	void stop() {
		// save state at finish
		isRunning = false;
		if (uiThread.joinable()) {
			uiThread.join();
		}
	}

private:
	ConcurrentHashMap& groupBuckets;
	std::chrono::milliseconds updateInterval;
	std::atomic<bool> isRunning;
	std::thread uiThread;

	void run() {
		Logger::log("UIUpdater::run");
		while (isRunning) {
			std::this_thread::sleep_for(updateInterval);
			if (!isRunning) {
				break;
			}
			auto snapshot = groupBuckets.snapshot();
			// TODO add UI interface render
			// renderUI(snapshot)
			Logger::log("size = " + std::to_string(snapshot.size()));
		}
		Logger::log("UIUpdater::stopped");
	};
};



#endif //UIUPDATER_HPP
