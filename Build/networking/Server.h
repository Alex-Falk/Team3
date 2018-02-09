//Alexander Falk
// 05/02/2018
// Handles Server functionality for the game. Includes Sending and Recieving updates to/from all clients

#pragma once
#include "User.h"

using namespace std;

class Server : public User
{
protected:
	Player * playerOne;
public:

	NetworkBase * server;
	GameTimer timer;

	Server();
	~Server() {
		server->Release();
		system("pause");
		exit(0);
	};

	int ServerLoop();

	NetworkBase * getBase() { return server; }

	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//

	std::string VectorToString(Vector3 v);
	//std::string ScoresToString(placeholder_scores s);

	//--------------------------------------------------------------------------------------------//
	// Sending / Broadcasting
	//--------------------------------------------------------------------------------------------//

	// need to send:
	// player info - positions/velocities/accelerations + Sizes
	// Player Scores
	// full map info - on connect -> use map index
	// changes to map

	void SendPosition();
	void SendLinVelocity();
	void SendAngVelocity();
	void SendAcceleration();
	void SendWeaponFire();
	void SendScores();
	void SendMap();
	void SendUpdatemap();

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

	void RecievePlayerAcceleration(string data, uint id); // this will be a direction vector to apply a force in
	void RecievePlayerPosition(string data, uint id);

	//--------------------------------------------------------------------------------------------//
	// Stored Variables
	//--------------------------------------------------------------------------------------------//

	// Stores:
	// Players
	// Scores
	// World
};

