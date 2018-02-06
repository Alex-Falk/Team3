#pragma once

#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>
#include <PC\GamePlay.h>
#include "NetworkCommon.h"

class Server
{
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
	std::string ScoresToString(placeholder_scores s);

	//--------------------------------------------------------------------------------------------//
	// Sending / Broadcasting
	//--------------------------------------------------------------------------------------------//

	// need to send:
	// player info - positions/velocities/accelerations + Sizes
	// Player Scores
	// full map info - on connect -> use map index
	// changes to map

	void SendPositions();
	void SendAccelerations();
	void SendScores();
	void SendMap();
	void SendUpdatemap();


	//--------------------------------------------------------------------------------------------//
	// Recieving
	//--------------------------------------------------------------------------------------------//

	// Updates from players

	void RecievePlayerAcceleration(string data, int id); // this will be a direction vector to apply a force in

	//--------------------------------------------------------------------------------------------//
	// Stored Variables
	//--------------------------------------------------------------------------------------------//

	// Stores:
	// Players
	// Scores
	// World
	placeholder_scores *	scores;
	placeholder_world *		world;
};

