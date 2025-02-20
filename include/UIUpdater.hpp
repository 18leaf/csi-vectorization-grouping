//
// Created by Lindsey on 2/15/2025.
//

#ifndef UIUPDATER_HPP
#define UIUPDATER_HPP
#include <atomic>
#include <chrono>
#include <iomanip>
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

	static std::string formatTimestamp(const std::chrono::system_clock::time_point& tp) {
		// formats timestamp to string for display
		std::time_t time = std::chrono::system_clock::to_time_t(tp);
		std::tm* tm = std::localtime(&time);
		std::ostringstream oss;
		oss << std::put_time(tm, "%H:%M:%S");
		return oss.str();
	}

	static std::string formatEmbedding(const std::vector<float>& embedding) {
		// vector embedding format to string
		std::ostringstream oss;
		for (size_t i = 0; i < embedding.size(); ++i) {
			oss << std::fixed << std::setprecision(3) << embedding[i];
			if (i < embedding.size() - 1) {
				oss << ", ";
			}
		}
		return oss.str();
	}

	void run() {
		Logger::log("UIUpdater::run");
		while (isRunning) {
			std::this_thread::sleep_for(updateInterval);
			if (!isRunning) {
				break;
			}
			auto snapshot = groupBuckets.snapshot();
			// add UI interface render
			// find abc bucket/group_id since this is what i created for the test data groupid
			if (snapshot.find("abc") == snapshot.end()) {
				std::cout << "No data for group 'abc' yet." << std::endl;
				continue;
			}
			// get bucket size for snapshot
			const auto& bucket = snapshot.at("abc");
			size_t bucketSize = bucket.size();

			std::cout << "--------------------- UI Update -------------------" << std::endl;
			std::cout << "Group: abc" << std::endl;
			std::cout << "Total messages processed: " << bucketSize << std::endl;

			// find start index to only print 3 newest entries
			// if bucket size greater than 3 start at bucketsize - 10
			size_t startIndex = (bucketSize >= 3) ? bucketSize - 3 : 0;

			std::cout << "Displaying newest " << bucketSize - startIndex << " messages:" << std::endl;
			for (size_t i = startIndex; i < bucketSize; ++i) {
				const auto& vecObj = bucket[i];
				std::string ts = formatTimestamp(vecObj.timestamp);
				std::string emb = formatEmbedding(vecObj.embedding);
				std::cout << "-----------------------------------------------------------------------------------------------------------\n{Time: " << ts
						  << " \n\nOriginal Message: " << vecObj.original_message
						  << " \n\n Embedding: [" << emb << "]}" << std::endl;
			}
			std::cout << "-------------------------------------------------" << std::endl;
		}
		Logger::log("UIUpdater::stopped");
	};
};



#endif //UIUPDATER_HPP
