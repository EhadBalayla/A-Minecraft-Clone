#include <enet/enet.h>
#include <iostream>
#include <thread>

ENetHost* server;
bool ThreadRunning = false;
void ServerThread() {
	ENetEvent event;
	while (ThreadRunning) {
		if (enet_host_service(server, &event, 100) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "A client connected from "
					<< event.peer->address.host
					<< ":" << event.peer->address.port << std::endl;
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "A client disconnected." << std::endl;
				break;
			default:
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
	
	server = enet_host_create(&address, 100, 2, 0, 0);
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
		std::cout << "type a command: ";
		std::cin >> cmd;

		if (cmd == "stop") running = false;
	}

	ThreadRunning = false;
	serverThread.join();




	enet_host_destroy(server);

	enet_deinitialize();
	return 0;
}