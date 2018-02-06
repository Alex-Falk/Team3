#include "Server.h"

#define CLIENT_ID evnt.peer->incomingPeerID

Server::Server() {

	server = new NetworkBase();
	printf("Server Initiated\n");

	//Win32_PrintAllAdapterIPAdresses();

	timer.GetTimedMS();
}

int Server::ServerLoop()
{
	while (true) 
	{
		float dt = timer.GetTimedMS() * 0.001f;

		server->ServiceNetwork(dt, [&](const ENetEvent& evnt) 
		{
			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf(" - New Client Connected\n");

				// Send over information to new client
			}
			break;

			case ENET_EVENT_TYPE_RECEIVE:

				string data = GetPacketData(evnt);
				PacketType type = FindType(data);
				switch (type) {
				case PLAYER_ACCELERATION:
				{
					RecievePlayerAcceleration(data, CLIENT_ID);
					break;
				}
				}
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
			{
				printf(" - Client %d has disconnected.\n", CLIENT_ID);
				break;
			}
			}
		});

	}
}



// message in: "enum: linx liny linz; angx angy angz"
void RecievePlayerAcceleration(string data, int id)
{
	int start = data.find_first_of(':');
	int mid = data.find_first_of(';');

	Vector3 linAcc = InterpretStringVector(data.substr(start, mid));
	Vector3 angAcc = InterpretStringVector(data.substr(mid + 2));
}