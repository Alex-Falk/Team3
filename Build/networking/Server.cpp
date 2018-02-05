#include "Server.h"

#define CLIENT_ID evnt.peer->incomingPeerID

Server::Server() {

	server = new NetworkBase();
	printf("Server Initiated\n");

	//Win32_PrintAllAdapterIPAdresses();

	timer.GetTimedMS();
}

int Server::ServerLoop() {
	while (true) {
		float dt = timer.GetTimedMS() * 0.001f;

		server->ServiceNetwork(dt, [&](const ENetEvent& evnt) {
			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf(" - New Client Connected\n");

				// Send over information to new client
			}
			break;

			case ENET_EVENT_TYPE_RECEIVE:

			case ENET_EVENT_TYPE_DISCONNECT:
			{
				printf(" - Client %d has disconnected.\n", CLIENT_ID);
				break;
			}
			}
		});

	}


}