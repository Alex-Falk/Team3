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
			string data = GetPacketData(evnt);
			PacketType type = FindType(data);

			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf(" - New Client Connected\n");

				// Send over information to new client
			}
			break;

			case ENET_EVENT_TYPE_RECEIVE:
				
				switch (type) {
				case PLAYER_ACCELERATION:
				{
					RecievePlayerAcceleration(data, CLIENT_ID);
					SendAccelerations(CLIENT_ID);
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
void Server::RecievePlayerAcceleration(string data, int id)
{
	size_t start = data.find_first_of(':');
	size_t mid = data.find_first_of(';');

	Vector3 linAcc = InterpretStringVector(data.substr(start, mid));
	Vector3 angAcc = InterpretStringVector(data.substr(mid + 2));

	// Apply forces accordingly

}

void Server::SendAccelerations(int id)
{
	string data;

	// Create a string containing the type of message, user to update and new updated vector
	//TODO replace the zero vector with the player's acceleration
	data = to_string(PLAYER_ACCELERATION) + ":" +
		to_string(id) + ";" + Vector3ToString(game->GetPlayer(id)->Physics()->GetAcceleration());

	ENetPacket* accPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_host_broadcast(server->m_pNetwork, 0, accPacket);
}