//
// Created by Lindsey on 2/15/2025.
//

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
// Thread Safe Shared Message Queue
class MessageQueue {
public:
	// enqueue an item
	void enqueue(const T& item) {
		// lock guard mutex lock when leaves scope
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.push(item);
		_cv.notify_one();
	}

	//dequeue item
	// had issues with blocking dequeue.. if there were no items left the cv wait got this stuck and didnt let thread exit
	T dequeue() {
		// unique lock = prevents multiple threads from accessing same item
		std::unique_lock<std::mutex> lock(_mutex);
		// condition variable block thread if queue is empty
		_cv.wait(lock, [this] { return !_queue.empty(); });
		T item = _queue.front();
		_queue.pop();
		return item;
	}

	bool try_dequeue(T& item, std::chrono::milliseconds timeout) {
		// updates item and return false if emtpy queue
		std::unique_lock<std::mutex> lock(_mutex);
		if (!_cv.wait_for(lock, timeout, [this] { return !_queue.empty(); })) {
			return false;
		}
		item = _queue.front();
		_queue.pop();
		return true;
	}

private:
	std::queue<T> _queue;
	std::mutex _mutex;
	std::condition_variable _cv;
};

#endif //MESSAGE_QUEUE_H
