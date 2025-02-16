//
// Created by Lindsey on 2/15/2025.
//

#ifndef IPC_CHANNEL_H
#define IPC_CHANNEL_H

#include <string>

class IPCChannel {
public:
	virtual ~IPCChannel() = default;
	// read raw data from channel
	virtual std::string read() = 0;

};

#endif //IPC_CHANNEL_H
