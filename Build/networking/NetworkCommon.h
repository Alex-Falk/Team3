#pragma once
#include <nclgl\common.h>
#include <PC\Player.h>
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
	void SetScore(uint id, int score);
	void SetAmmo(uint id, float ammo);
	void SetSize(uint id, float size);
	void LoadLevel(uint levelID);
	void SetAcceleration(uint id, Vector3 a);
	void SetPosition(uint id, Vector3 p);

	Player * GetPlayer(uint id);
	int GetMapIndex();

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
