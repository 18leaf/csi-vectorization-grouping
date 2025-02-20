//
// Created by Lindsey on 2/15/2025.
//

#ifndef WORKERPOOL_HPP
#define WORKERPOOL_HPP
#include "concurrent_hashmap.h"
#include "logger.h"
#include "message.h"
#include "message_queue.h"
#include "vectorizer.h"


// worker pool is responsible for creating embeddings for the messages and storing them in the hashmap
class WorkerPool {
public:
	WorkerPool(MessageQueue<Message>& queue, ConcurrentHashMap& buckets, Vectorizer* vec, size_t numThreads)
		: message_queue(queue), group_buckets(buckets), vectorizer(vec), isRunning(true), workerThreads(numThreads), numThreads(numThreads) {
		// reserve space for n threads (once we start)
		workerThreads.reserve(numThreads);
	}

	~WorkerPool() {
		stop();
	}

	void start() {
		// deploy each worker to their own running loop
		// populate the reserved workerThreads
		for (size_t i = 0; i < numThreads; i++) {
			workerThreads.emplace_back(&WorkerPool::runWorker, this);
		}
	}

	void stop() {
		isRunning = false;
		for (auto &thread : workerThreads) {
			// join threads
			if (thread.joinable()) {
				thread.join();
			}
		}
	}

private:
	MessageQueue<Message>& message_queue;
	// concurrent hashmap for a bucket for each group
	ConcurrentHashMap& group_buckets;
	Vectorizer* vectorizer;
	std::atomic<bool> isRunning;
	std::vector<std::thread> workerThreads;
	const size_t numThreads;

	void runWorker() {
		// loop for each worker thread to execute
		Logger::log("WorkerPool::run");
		Message msg;
		// begin
		while (isRunning) {
			// uses safe dequeue from message_queue.h and assign to msg
			// check if message recieved and updates message as well
			bool gotMessage = message_queue.try_dequeue(msg, std::chrono::milliseconds(250));
			if (!gotMessage) {
				// if message not received loop again
				continue;
			}
			// vectorize message content and assign to embedding
			std::vector<float> embedding = vectorizer->vectorize(msg.content);
			// add a sleep to simulate more processing here
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			//create Vector Object
			VectorObject vector_object;
			vector_object.embedding = embedding;
			vector_object.group_id = msg.group_id;
			vector_object.timestamp = msg.timestamp;
			vector_object.original_message = msg.content;

			// safely append vector object to the assigned group buckets
			group_buckets.append(msg.group_id, vector_object);

		}

		Logger::log("WorkerPool::Stopped");
	}
};




#endif //WORKERPOOL_HPP
