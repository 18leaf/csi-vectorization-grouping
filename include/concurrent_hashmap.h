//
// Created by Lindsey on 2/15/2025.
//

#ifndef CONCURRENT_HASHMAP_H
#define CONCURRENT_HASHMAP_H

#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>

#include "message.h" //VectorObject

class ConcurrentHashMap {
public:
	// adds vector embedding to key
	void append(const std::string& key, const VectorObject& value) {
		// shared_mutex -> unique lock enforces only 1 thread can do this now
		std::unique_lock lock(mutex_);
		// add to end of list
		map_[key].push_back(value);
	};

	// get snapshot of bucket from group_id
	std::vector<VectorObject> get_bucket(const std::string& key) const {
		std::shared_lock lock(mutex_);
		const auto it = map_.find(key);
		// return the value of key (it->second) or empty object if none
		return (it != map_.end()) ? it->second : std::vector<VectorObject>();
	};

	// get bucket size
	size_t bucket_size(const std::string& key) const {
		std::shared_lock lock(mutex_);
		const auto it = map_.find(key);
		return (it != map_.end()) ? it->second.size() : 0;
	};

	// get entire snapshot of map
	std::unordered_map<std::string, std::vector<VectorObject>> snapshot() const {
		std::shared_lock lock(mutex_);
		return map_;
	};

private:
	// hashmap of group_id -> (embedding1, embedding2, ..., embedding n)
	std::unordered_map<std::string, std::vector<VectorObject>> map_;
	// shared_mutex so that I can allow controlled access of multiple threads
	mutable std::shared_mutex mutex_;
};



#endif //CONCURRENT_HASHMAP_H
