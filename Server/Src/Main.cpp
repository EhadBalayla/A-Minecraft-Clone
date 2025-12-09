#include <enet/enet.h>
#include <iostream>
#include <thread>

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
				dvec3* data = reinterpret_cast<dvec3*>(event.packet->data);			

				std::cout << "a player is a: X - " << (int)data->x << ", Y - " << (int)data->y << ", Z - " << (int)data->z << std::endl;
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