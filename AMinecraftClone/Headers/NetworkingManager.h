#pragma once
#include <enet/enet.h>
#include <thread>
#include <condition_variable>
#include <mutex>



enum class NetworkingState {
	Disconnected,
	Connecting,
	Connected,
	Disconnecting
};

struct status {
	NetworkingState state;
	const char* ip;
	int port;
};

class NetworkingManager {
public:
	void Init();
	void Terminate();

	void Connect(const char* address, int port);
	void Disconnect();

	status GetCurrentStatus();
private:
	ENetHost* client = nullptr;
	ENetPeer* peer = nullptr;

	//status stuff
	NetworkingState state = NetworkingState::Disconnected;
	const char* ConnectedIP;
	int port;
	
	//networking thread
	void NetworkThread();
	bool threadRunning = false;
	std::thread networkThread;
	std::mutex tempMTX;
	std::condition_variable tempCV;
};