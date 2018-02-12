#include "Server.h"
#include <PC/Game.h>
#define CLIENT_ID evnt.peer->incomingPeerID

Server::Server() {

	server = new NetworkBase();
	printf("Server Initiated\n");

	server->Initialize(1234, 4);

	userID = 0;
	timer.GetTimedMS();
}

void Server::updateUser(float dt)
{
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
			case PLAYER_POS:
			{
				Game::Instance()->SetPosition(CLIENT_ID,ReceivePosition(data).v);
				break;
			}
			case PLAYER_LINVEL:
			{
				Game::Instance()->SetLinearVelocity(CLIENT_ID,ReceiveLinVelocity(data).v);
				break;
			}
			case PLAYER_ANGVEL:
			{
				Game::Instance()->SetAngularVelocity(CLIENT_ID,ReceiveAngVelocity(data).v);
				break;
			}
			case PLAYER_ACCELERATION:
			{
				Game::Instance()->SetAcceleration(CLIENT_ID,ReceiveAcceleration(data).v);
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

	for (uint i = 0; i < 4; ++i)
	{
		SendPosition(i);
		SendLinVelocity(i);
		SendAngVelocity(i);
		SendAcceleration(i);
		//SendScores();
	}

}

//--------------------------------------------------------------------------------------------//
// Recieving
//--------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Server::SendPosition(uint ID) 
{
	string data;

	data = to_string(PLAYER_POS) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->Physics()->GetPosition());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendLinVelocity(uint ID)
{
	string data;

	data = to_string(PLAYER_LINVEL) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->Physics()->GetLinearVelocity());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAngVelocity(uint ID)
{
	string data;

	data = to_string(PLAYER_ANGVEL) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->Physics()->GetAngularVelocity());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAcceleration(uint ID)
{
	string data;

	data = to_string(PLAYER_ACCELERATION) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->Physics()->GetAcceleration());

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


