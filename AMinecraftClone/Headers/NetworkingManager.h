#pragma once
#include <enet/enet.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <glm/glm.hpp>



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

	void Render();

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

	//the player info
	std::mutex playerPosMTX; //to protect on it when we render so we wont change data while rendering
	uint32_t otherPlayersCount = 10;
	glm::dvec3 playersPos[10] = { {0.0, 0.0, 0.0} };
};