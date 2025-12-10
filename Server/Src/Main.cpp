#include <enet/enet.h>
#include <iostream>
#include <thread>

int connected_peers_count(ENetHost* host) {
	int count = 0;
	for (size_t i = 0; i < host->peerCount; ++i) {
		if (host->peers[i].state == ENET_PEER_STATE_CONNECTED) count++;
	}
	return count;
}


struct dvec3 {
	double x;
	double y;
	double z;
};

ENetHost* server;
bool ThreadRunning = false;
void ServerThread() {
	ENetEvent event;
	while (ThreadRunning) {
		while (enet_host_service(server, &event, 100) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				ENetPeer* sender = event.peer;

				for (size_t i = 0; i < server->peerCount; i++) {
					ENetPeer* peer = &server->peers[i];

					if (peer->state != ENET_PEER_STATE_CONNECTED) continue;
					if (peer == sender) continue;

					uint8_t buffer[1 + sizeof(dvec3)];
					buffer[0] = sender->incomingPeerID;
					memcpy(buffer + 1, event.packet->data, sizeof(dvec3));


					ENetPacket* packet = enet_packet_create(buffer, sizeof(buffer), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);

					enet_peer_send(peer, 0, packet);
				}

				enet_packet_destroy(event.packet);
				break;
			}
		}
	}
}
int main() {
	if (enet_initialize() != 0) {
		std::cerr << "an error occured when initializing ENET" << std::endl;
		return EXIT_FAILURE;
	}

	ENetAddress address;
	enet_address_set_host(&address, "127.0.0.1");
	address.port = 25565;
	
	server = enet_host_create(&address, 10, 2, 0, 0);
	if (!server) {
		std::cerr << "an error occured when trying to create a server host" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "server started at address: " << address.host << ", on port: " << address.port << std::endl;
	
	std::cout << "starting thread" << std::endl;
	
	ThreadRunning = true;
	std::thread serverThread = std::thread(&ServerThread);

	bool running = true;
	std::string cmd;
	while (running) {
		std::cin >> cmd;

		if (cmd == "stop") running = false;
	}

	ThreadRunning = false;
	serverThread.join();




	enet_host_destroy(server);

	enet_deinitialize();
	return 0;
}