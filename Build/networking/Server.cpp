#include "Server.h"

#define CLIENT_ID evnt.peer->incomingPeerID

Server::Server() {

	server = new NetworkBase();
	printf("Server Initiated\n");

	//Win32_PrintAllAdapterIPAdresses();

	userID = 0;
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
					SendAcceleration(CLIENT_ID);
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

//--------------------------------------------------------------------------------------------//
// Recieving
//--------------------------------------------------------------------------------------------//

// message in: "enum: linx liny linz"
void Server::RecievePlayerAcceleration(string data, uint ID)
{
	size_t start = data.find_first_of(':');
	size_t mid = data.find_first_of(';');

	Vector3 linAcc = InterpretStringVector(data.substr(start, mid));
	Vector3 angAcc = InterpretStringVector(data.substr(mid + 2));

	game->GetPlayer(ID)->Physics()->SetAcceleration(linAcc);

}

void Server::RecievePlayerPosition(string data, uint ID)
{
	size_t start = data.find_first_of(':');

	Vector3 pos = InterpretStringVector(data.substr(start));

	game->GetPlayer(ID)->Physics()->SetPosition(pos);
}

//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Server::SendPosition(uint ID) 
{
	string data;

	data = to_string(PLAYER_POS) + ":" +
		to_string(ID) + ";" + Vector3ToString(game->GetPlayer(ID)->Physics()->GetPosition());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendLinVelocity(uint ID)
{
	string data;

	data = to_string(PLAYER_LINVEL) + ":" +
		to_string(ID) + ";" + Vector3ToString(game->GetPlayer(ID)->Physics()->GetLinearVelocity());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAngVelocity(uint ID)
{
	string data;

	data = to_string(PLAYER_ANGVEL) + ":" +
		to_string(ID) + ";" + Vector3ToString(game->GetPlayer(ID)->Physics()->GetAngularVelocity());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAcceleration(uint ID)
{
	string data;

	data = to_string(PLAYER_ACCELERATION) + ":" +
		to_string(ID) + ";" + Vector3ToString(game->GetPlayer(ID)->Physics()->GetAcceleration());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

//TODO WEAPONS stuff

//PACKET_TYPE:WEAPON_TYPE;XPOS YPOS ZPOS, XDIR YDIR ZDIR
void Server::SendWeaponFire(uint ID) 
{
	string data;

	data = to_string(PLAYER_WEAPON) + ":";
}