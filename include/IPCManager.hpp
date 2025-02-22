//
// Created by Lindsey on 2/15/2025.
//

#ifndef IPCMANAGER_H
#define IPCMANAGER_H
#include "ipc_channel.h"
#include "logger.h"
#include "message.h"
#include "message_queue.h"
#include <thread>
#include <atomic>
#include <sstream>
#include <string>
#include <chrono>


class IPCManager {
// create thread for processes related to ipc -> connect to ipc channel, read from and validate, send to messagequeue
public:
	IPCManager(IPCChannel *channel, MessageQueue<Message>& queue)
		: ipcChannel(channel), msgQueue(queue), isRunning(true) {}

	~IPCManager() {
		// safely end process
		stop();
	};

	void start() {
		// start thread on the run() function
		ipcThread = std::thread(&IPCManager::run, this);
	};

	void stop() {
		isRunning = false;
		if (ipcThread.joinable()) {
			ipcThread.join();
		}
	}
private:
	IPCChannel *ipcChannel;
	MessageQueue<Message>& msgQueue;
	// flag for thread status
	std::atomic<bool> isRunning;
	std::thread ipcThread;

	static bool parseCSVLine(const std::string &line, Message &msg) {
		// iterate over line, break on ",", assign first to group_id, second to timestamp, finally content
		std::istringstream iss(line);
		std::string token;
		if (std::getline(iss, token, ',')) {
			// check for first csv value, which should line up to group_id as a str, then assign to msg
			msg.group_id = token;
		} else return false;

		if (std::getline(iss, token, ',')) {
			// below is ideal, but for simplicity i am setting to current time -> dont have to worry abt snyrhonization with other process
			//msg.timestamp = token;
			// TODO actually implement message sent timestamp
			msg.timestamp = std::chrono::system_clock::now();
		} else return false;

		// consider adding delim to \n to check for end line in case content has any other chars in it
		if (std::getline(iss, token)) {
			msg.content = token;
		} else return false;

		// all parts of msg should be populated and validated
		return true;
	};

	// loop for IPC Manager
	void run() {
		Logger::log("IPCManager::run");
		while (isRunning) {
			// read line from ipcChannel, check if empty
			std::string line = ipcChannel->read();
			// sleep for 20 ms to simulate delay
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			if (!line.empty()) {
				//create new msg object
				// boolean parse csv (also updates Message msg with parsed content) from format group_id, timestamp, contents
				// TODO add data corruption/verification methods
				if (Message msg; parseCSVLine(line, msg)) {
					// if msg was parsed and valid continue
					msgQueue.enqueue(msg);
				} else {
					// something wrong happened with parsing that line
					Logger::log("IPCManager::Parse Error - " + line);
				}
			} else {
				// sleep for n milliseconds if no lines exist (give time for more data to pop up)
				// set to 1 second for now
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
		Logger::log("IPCManager::stopped");
	}
};


#endif //IPCMANAGER_H
