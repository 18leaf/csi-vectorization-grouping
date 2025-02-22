//
// Created by Lindsey on 2/22/2025.
//

#include "../include/message_queue.h"
#include <iostream>
#include <thread>
#include <chrono>

// DEMO for the message_queue.h dequeue() function
// which can lead to deadlock depending on implementation of worker threads
// note that I resolved this by introducing a try_dequeue function that
// has a timeout mechanism

int main() {
	MessageQueue<int> mq;

	// thread 1 with blocking dequeue (simple dequeue() function)
    // will block indefineitely if there is no msg to enqueue
	std::thread t1([&mq]() {
		std::cout << "t1::attempt blocking deqeue" << std::endl;
		int value = mq.dequeue();  // WILL WAIT INDEFINITELY if no item is available
		// notice how t1::value will not happen while blocked until after line 53 executes (mq.enqueue)
		std::cout << "t1::value" << value << std::endl;
	});

	// wait to show no producer workign
	std::this_thread::sleep_for(std::chrono::seconds(2));

	// thread 2 -> demonstrates try_dequeue implementing a timeout resolution
	std::thread t2([&mq]() {
		std::cout << "t2::attempt try_dequeue" << std::endl;
		int value;
		bool success = mq.try_dequeue(value, std::chrono::milliseconds(500));
		if (success) {
			std::cout << "t2::value: " << value << std::endl;
		} else {
			std::cout << "t2::timeout occured no value dequeued" << std::endl;
		}
	});

	t2.join();

	// enqueue value to unblock dequeue
	std::cout << "Main::enqueing value to unblock t1" << std::endl;
	mq.enqueue(42);

	t1.join();
	std::cout << "Main::Finished" << std::endl;
	return 0;
}