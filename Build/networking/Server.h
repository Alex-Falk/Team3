//Alexander Falk
// 05/02/2018
// Handles Server functionality for the game. Includes Sending and Recieving updates to/from all clients

#pragma once
#include "User.h"
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")


using namespace std;

class Server : public User
{
public:

	NetworkBase * server;
	GameTimer timer;
	uint serverPort = 1234;

	Server();
	~Server() {
		server->Release();
		system("pause");
		exit(0);
	};

	void updateUser(float dt);

	NetworkBase * getBase() { return server; }

	//--------------------------------------------------------------------------------------------//
	// Sending / Broadcasting
	//--------------------------------------------------------------------------------------------//

	// need to send:
	// player info - positions/velocities/accelerations + Sizes
	// Player Scores
	// full map info - on connect -> use map index
	// changes to map

	void SendPosition(uint ID);
	void SendLinVelocity(uint ID);
	void SendAngVelocity(uint ID);
	void SendAcceleration(uint ID);
	void SendWeaponFire(uint ID);
	//void SendScores();
	//void SendMap();
	//void SendUpdatemap();

	std::string GetPacketData(const ENetEvent & evnt)
	{
		std::string out;
		for (int i = 0; i < evnt.packet->dataLength; ++i) {
			out.push_back(evnt.packet->data[i]);
		}

		return out;
	}

	//--------------------------------------------------------------------------------------------//
	// Recieving
	//--------------------------------------------------------------------------------------------//

	// Updates from players

	//--------------------------------------------------------------------------------------------//
	// Stored Variables
	//--------------------------------------------------------------------------------------------//

	// Stores:
	// Players
	// Scores
	// World
};

