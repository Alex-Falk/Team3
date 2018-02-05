#pragma once

#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>

enum placeholder_colours {
	RED,
	BLUE,
	GREEN,
	PINK
};

struct placeholder_scores {
	unsigned short player1;
	unsigned short player2;
	unsigned short player3;
	unsigned short player4;
};

class placeholder_player;
class placeholder_world;

enum PacketType {
	GAME_START,
	PLAYER_POS,
	PLAYER_ACCELERATION,
	PLAYER_SIZES,
	SCORES,
	MAP_INDEX,
	MAP_UPDATE,
	GAME_END
};


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

	PacketType FindType(ENetEvent evnt);

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

	void RecievePlayerInput(); // this will be a direction vector to apply a force in

	//--------------------------------------------------------------------------------------------//
	// Stored Variables
	//--------------------------------------------------------------------------------------------//

	// Stores:
	// Players
	// Scores
	// World
	placeholder_scores *	scores;
	placeholder_world *		world;
	placeholder_player **	players;
};

