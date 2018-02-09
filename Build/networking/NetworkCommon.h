//Alexander Falk
// 05/02/2018
// Common functions/enums used both by the server and the client

#pragma once
#include <nclgl\common.h>
#include <enet\enet.h>
#include <PC\Player.h>

enum PacketType {
	GAME_START,				// Informs clients the game has started
	PLAYER_POS,				// Server->Client: Broadcast positions.		Client->Server: Inform on Update
	PLAYER_LINVEL,
	PLAYER_ANGVEL,
	PLAYER_ACCELERATION,	// Server->Client: Broadcast.				Client->Server:			"
	PLAYER_SIZES,			// Server->Client: Broadcast player sizes	
	PLAYER_WEAPON,			// Server->Client: Broadcast pos/dir		Client->Server:	Inform of spawn pos/dir
	SCORES,					// Server->Client: Broadcast Scores
	MAP_INDEX,				// Server->Client: Boradcast Map to load
	MAP_UPDATE,				//
	GAME_END				// Server->Client: Informs clients game has ended
};

struct IP {
	int a;
	int b;
	int c;
	int d;
	int port;
};

//--------------------------------------------------------------------------------------------//
// PLACEHOLDERS
//--------------------------------------------------------------------------------------------//

// Setters
struct Placeholder_Game {

	// Setting player stuff
	void SetScore(uint id, int score);
	void SetAmmo(uint id, float ammo);
	void SetSize(uint id, float size);
	void SetAcceleration(uint id, Vector3 a);
	void SetPosition(uint id, Vector3 p);
	
	void SetMyId(uint id);

	// Getters
	uint GetMyId();
	Player * GetPlayer(uint id);
	Player * GetMyPlayer();
	int GetMapIndex();

	// General Game Functions
	void InitializeMatch();
	void LoadLevel(uint levelID);
	void StartMatch();
	void EndMatch();

};

struct Placeholder_Map {
	int numObjects;
};


static Player * playerOne;
static Placeholder_Game * game;
static int placeholder_playerNum = 4;

//--------------------------------------------------------------------------------------------//
// Functions
//--------------------------------------------------------------------------------------------//

// Take a string in the format: "x.xx y.yy z.zz"
Vector3 InterpretStringVector(string message);

string Vector3ToString(Vector3 in);

// Function to split a string into a vector of strings by a delimiter d
vector<string> split_string(string s, char d);

PacketType FindType(string data);

ENetPacket* CreatePacket(string data) {
	return enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
}
