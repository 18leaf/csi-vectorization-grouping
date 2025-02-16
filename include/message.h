//
// Created by Lindsey on 2/15/2025.
//

#ifndef MESSAGE_H
#define MESSAGE_H

#include <chrono>
#include <string>


// received message structure
struct Message {
	// std::string message_id
	std::string group_id;
	std::chrono::system_clock::time_point timestamp;
	std::string content;
};

struct VectorObject {
	std::vector<float> embedding;
	std::string group_id;
	std::chrono::system_clock::time_point timestamp;
	// std::string message_id
};

#endif //MESSAGE_H
