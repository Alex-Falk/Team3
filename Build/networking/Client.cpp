#include "Client.h"
#include <nclgl\NCLDebug.h>

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
		&Client::ProcessNetworkEvent,	// Function to call
		this,								// Associated class instance
		std::placeholders::_1);				// Where to place the first parameter
	network.ServiceNetwork(dt, callback);
}


void Client::ProcessNetworkEvent(const ENetEvent& evnt)
{
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
		PacketType type = FindType(evnt);
		switch (type) {
		case GAME_START:
		{
			break;
		}
		case PLAYER_POS:
		{
			ReceivePositions(evnt);
			break;
		}
		case PLAYER_ACCELERATION:
		{
			RecieveAcceleration(evnt);
			break;
		}
		case PLAYER_SIZES:
		{
			RecieveSizes(evnt);
			break;
		}
		case SCORES:
		{
			RecieveScores(evnt);
			break;
		}
		case MAP_INDEX:
		{
			RecieveMapIndex(evnt);
			break;
		}
		case MAP_UPDATE:
		{
			RecieveMapChange(evnt);
			break;
		}
		case GAME_END:
		{
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