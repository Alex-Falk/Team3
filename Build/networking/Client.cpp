#include "Client.h"

const Vector3 status_color3 = Vector3(1.0f, 0.6f, 0.6f);

Client::Client()
{
	if (network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");
		//Attempt to connect to the server on localhost:1234
		serverConnection = network.ConnectPeer(127, 0, 0, 1, 1234);
		NCLDebug::Log("Network: Attempting to connect to server.");
	}


}

Client::Client(IP ip) {
	if (network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");
		//Attempt to connect to the server on localhost:1234
		serverConnection = network.ConnectPeer(ip.a, ip.b, ip.c, ip.d, ip.port);
		NCLDebug::Log("Network: Attempting to connect to server.");
	}

	game->SetMyId(CLIENT_ID);
}


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
			game->InitializeMatch();
			break;
		}
		case PLAYER_POS:
		{
			ReceivePositions(data);
			break;
		}
		case PLAYER_ACCELERATION:
		{
			RecieveAcceleration(data);
			break;
		}
		case PLAYER_SIZES:
		{
			RecieveSizes(data);
			break;
		}
		case SCORES:
		{
			RecieveScores(data);
			break;
		}
		case MAP_INDEX:
		{
			RecieveMapIndex(data);
			break;
		}
		case MAP_UPDATE:
		{
			RecieveMapChange(data);
			break;
		}
		case GAME_END:
		{
			game->EndMatch();
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

void Client::ReceivePositions(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx+1, semicolonIdx));

	string p = data.substr(semicolonIdx);
	Vector3 pos = InterpretStringVector(p);

	game->SetPosition(playerID, pos);
}

void Client::RecieveAcceleration(string data) 
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx+1, semicolonIdx ));

	string a = data.substr(semicolonIdx);
	Vector3 acceleration = InterpretStringVector(a);

	game->SetAcceleration(playerID, acceleration);
}

void Client::RecieveSizes(string data)
{
	string s = data.substr(data.find_first_of(':') + 1);
	vector<string> splitData = split_string(s, ' ');

	for (uint i = 0; i < placeholder_playerNum; ++i) 
	{
		game->SetSize(i, stof(splitData[i]));
	}

}

void Client::RecieveScores(string data) 
{

	string s = data.substr(data.find_first_of(':') + 1);
	vector<string> splitData = split_string(s, ' ');

	for (uint i = 0; i < placeholder_playerNum; ++i)
	{
		game->SetScore(i, stoi(splitData[i]));
	}
	
}

void Client::RecieveMapIndex(string data)
{
	string s = data.substr(data.find_first_of(':') + 1);
	uint mapIndex = stoi(s);

	game->LoadLevel(mapIndex);
}




void Client::SendPosition()
{
	Vector3 pos = game->GetMyPlayer()->Physics()->GetPosition();

	string data = Vector3ToString(pos);

	ENetPacket* posPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, posPacket);
}

void Client::SendVelocity()
{

}

void Client::SendAcceleration()
{
	Vector3 acc = game->GetMyPlayer()->Physics()->GetAcceleration();

	string data = Vector3ToString(acc);

	ENetPacket* accPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, accPacket);
}