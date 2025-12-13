#include "NetworkingManager.h"
#include <iostream>
#include "Game.h"

void NetworkingManager::Init() {
	if (enet_initialize() != 0) {
		std::cout << "an error occured while trying to initalize ENET" << std::endl;
		exit(EXIT_FAILURE);
	}

	client = enet_host_create(NULL, 1, 2, 0, 0);
	if (!client) {
		std::cout << "An error occurred while trying to create a client host." << std::endl;
		exit(EXIT_FAILURE);
	}


	threadRunning = true;
	networkThread = std::thread(&NetworkingManager::NetworkThread, this);
}
void NetworkingManager::Terminate() {
	threadRunning = false;
	tempCV.notify_one();
	networkThread.join();

	enet_host_destroy(client);

	enet_deinitialize();
}

void NetworkingManager::Connect(const char* hostAddress, int pPort) {
	if (state != NetworkingState::Disconnected) return;

	ConnectedIP = "127.0.0.1";
	port = pPort;

	tempCV.notify_one();
}
void NetworkingManager::Disconnect() {
	if (state != NetworkingState::Connected) return;

	state = NetworkingState::Disconnecting; //if it is on the connected state, the networking thread will never change the variable it will only read from it, so its safe
}

void NetworkingManager::Render() {
	if (otherPlayersCount > 0 && state == NetworkingState::Connected) {

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		Game::e_DummyPlayersShader.use();
		Game::e_DummyPlayersShader.setMat4("view", Game::View);
		Game::e_DummyPlayersShader.setMat4("proj", Game::Proj);
		for (int i = 0; i < otherPlayersCount; i++) {
			glm::vec3 relativePos = playersPos[i] - Game::player.GetPosition();
			Game::e_OpaqueShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));
			glBindVertexArray(Game::tempVAO);
			glDrawArrays(GL_TRIANGLES, 0, 24);
		}
	}
}


status NetworkingManager::GetCurrentStatus() {
	return { state, ConnectedIP, port };
}


void NetworkingManager::NetworkThread() {
	while (threadRunning) {
		switch (state) {
		case NetworkingState::Disconnected:
		{
			if (!threadRunning) return;

			{
				std::unique_lock<std::mutex> lock(tempMTX);
				tempCV.wait(lock);
			}

			if (!threadRunning) return;

			state = NetworkingState::Connecting;
			break;
		}
		case NetworkingState::Connecting: {
			ENetAddress address;
			ENetEvent connectionEvent;
			enet_address_set_host(&address, ConnectedIP);
			address.port = port;

			peer = enet_host_connect(client, &address, 2, 0);

			if (!peer) {
				//if the server is full
				state = NetworkingState::Disconnected;
				if (!threadRunning) return;
				break;
			}

			if (!threadRunning) return;
			if (enet_host_service(client, &connectionEvent, 1000) > 0 && connectionEvent.type == ENET_EVENT_TYPE_CONNECT) {
				state = NetworkingState::Connected;
			}
			else {
				//if failed then forcing disconnect
				enet_peer_reset(peer);
				state = NetworkingState::Disconnected;
			}
			if (!threadRunning) return;
			break;
		}
		case NetworkingState::Connected: {
			ENetEvent event;
			glm::dvec3 playerPos = Game::player.GetPosition();
			ENetPacket* playerCoordsPack = enet_packet_create(&playerPos, sizeof(glm::dvec3), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
			
			enet_peer_send(peer, 0, playerCoordsPack);

			while (enet_host_service(client, &event, 100) > 0) {
				switch (event.type) {
				case ENET_EVENT_TYPE_RECEIVE:
				{
					uint8_t* buffer = event.packet->data;

					uint8_t senderIDX = buffer[0];

					memcpy(&playersPos[senderIDX], buffer + 1, sizeof(glm::dvec3));

					enet_packet_destroy(event.packet);

					break;
				}
				}
			}

			
			break;
		}
		case NetworkingState::Disconnecting: {
			ENetEvent disconnectionEvent;
			enet_peer_disconnect(peer, 0);

			if (!threadRunning) return;
			if (!(enet_host_service(client, &disconnectionEvent, 1000) > 0 && disconnectionEvent.type == ENET_EVENT_TYPE_DISCONNECT)) enet_peer_reset(peer); //if disconnect fail then force quit
			state = NetworkingState::Disconnected;
			if (!threadRunning) return;
			break;
		}
		}
	}
}