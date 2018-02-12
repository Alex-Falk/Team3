#include "Client.h"
#include <PC/Game.h>
const Vector3 status_color3 = Vector3(1.0f, 0.6f, 0.6f);

Client::Client()
{
	if (network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");
		//Attempt to connect to the server on localhost:1234
		serverConnection = network.ConnectPeer(127, 0, 0, 1, 1234);
		NCLDebug::Log("Network: Attempting to connect to server.");

		userID = CLIENT_ID;
	}

	
}

//Client::Client(IP ip) {
//	if (network.Initialize(0))
//	{
//		NCLDebug::Log("Network: Initialized!");
//		//Attempt to connect to the server on localhost:1234
//		serverConnection = network.ConnectPeer(ip.a, ip.b, ip.c, ip.d, ip.port);
//		NCLDebug::Log("Network: Attempting to connect to server.");
//	}
//
//	userID = CLIENT_ID;
//}


Client::~Client()
{
	enet_peer_disconnect_now(serverConnection, 0);
	network.Release();
	serverConnection = NULL;
}

void Client::UpdateClient(float dt)
{
	//Update Network
	auto callback = std::bind(
		&Client::ProcessNetworkEvent,		// Function to call
		this,								// Associated class instance
		std::placeholders::_1);				// Where to place the first parameter
	network.ServiceNetwork(dt, callback);


}


void Client::ProcessNetworkEvent(const ENetEvent& evnt)
{
	string data = GetPacketData(evnt);
	switch (evnt.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
	{
		if (evnt.peer == serverConnection)
		{
			NCLDebug::Log(status_color3, "Network: Successfully connected to server!");
		}
		break;
	}
	
	case ENET_EVENT_TYPE_RECEIVE:
	{
		PacketType type = FindType(data);
		switch (type) {
		case GAME_START:
		{
			//Game::Instance()->InitializeMatch();
			break;
		}
		case PLAYER_POS:
		{
			PlayerVector pvec = ReceivePosition(data);
			temps.positions[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_LINVEL:
		{
			PlayerVector pvec = ReceiveLinVelocity(data);
			temps.linVelocities[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_ANGVEL:
		{
			PlayerVector pvec = ReceiveAngVelocity(data);
			temps.angVelocities[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_ACCELERATION:
		{
			PlayerVector pvec = ReceiveAcceleration(data);
			temps.accelerations[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_SIZES:
		{
			ReceiveSizes(data);
			break;
		}
		case SCORES:
		{
			ReceiveScores(data);
			break;
		}
		case MAP_INDEX:
		{
			ReceiveMapIndex(data);
			break;
		}
		case MAP_UPDATE:
		{
			//ReceiveMapChange(data);
			break;
		}
		case GAME_END:
		{
			//Game::Instance()->EndMatch();
			break;
		}
		}
		break;
	}
	case ENET_EVENT_TYPE_DISCONNECT:
	{
		break;
	}

	}
}

//--------------------------------------------------------------------------------------------//
// Recieving
//--------------------------------------------------------------------------------------------//

void Client::ReceiveSizes(string data)
{
	string s = data.substr(data.find_first_of(':') + 1);
	vector<string> splitData = split_string(s, ' ');

	for (uint i = 0; i < placeholder_playerNum; ++i) 
	{
		Game::Instance()->SetSize(i, stof(splitData[i]));
	}

}

void Client::ReceiveScores(string data) 
{

	string s = data.substr(data.find_first_of(':') + 1);
	vector<string> splitData = split_string(s, ' ');

	for (uint i = 0; i < placeholder_playerNum; ++i)
	{
		Game::Instance()->SetScore(i, stoi(splitData[i]));
	}
	
}

void Client::ReceiveMapIndex(string data)
{
	string s = data.substr(data.find_first_of(':') + 1);
	uint mapIndex = stoi(s);

	//Game::Instance()->LoadLevel(mapIndex);
}


//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Client::SendPosition(uint ID)
{
	Vector3 pos = Game::Instance()->GetPlayer(userID)->Physics()->GetPosition();

	string data = Vector3ToString(pos);

	ENetPacket* posPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, posPacket);
}

void Client::SendLinVelocity(uint ID)
{
	Vector3 vel = Game::Instance()->GetPlayer(userID)->Physics()->GetLinearVelocity();

	string data = Vector3ToString(vel);

	ENetPacket* velPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, velPacket);
}

void Client::SendAngVelocity(uint ID)
{
	Vector3 vel = Game::Instance()->GetPlayer(userID)->Physics()->GetAngularVelocity();

	string data = Vector3ToString(vel);

	ENetPacket* velPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, velPacket);
}

void Client::SendAcceleration(uint ID)
{
	Vector3 acc = Game::Instance()->GetPlayer(userID)->Physics()->GetAcceleration();

	string data = Vector3ToString(acc);

	ENetPacket* accPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, accPacket);
}

void Client::SendWeaponFire(uint ID)
{

}