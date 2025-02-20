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
// real for linux synthetic for windows for testing
#ifdef _WIN32
IPCChannel* createSyntheticIPCChannel(const std::string& filepath);
#else
IPCChannel* createRealIPCChannel(const std::string& pipePath);
#endif


#endif //IPC_CHANNEL_H
