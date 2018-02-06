#pragma once
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

//--------------------------------------------------------------------------------------------//
// PLACEHOLDERS
//--------------------------------------------------------------------------------------------//

struct placeholder_scores {
	unsigned short playerOne;
	unsigned short playerTwo;
	unsigned short playerThree;
	unsigned short playerFour;
};

class placeholder_world;


Player * playerOne;
Player * playerTwo;
Player * playerThree;
Player * playerFour;

//--------------------------------------------------------------------------------------------//
// Functions
//--------------------------------------------------------------------------------------------//

// Take a string in the format: "x.xx y.yy z.zz"
Vector3 InterpretStringVector(string message) 
{
	Vector3 out;

	int xIdx = message.find_first_of(' ');
	int yIdx = message.substr(xIdx).find_first_of(' ');

	out.x = stof(message.substr(0, xIdx));
	out.y = stof(message.substr(xIdx+1, yIdx));
	out.z = stof(message.substr(yIdx+1));

	return out;
}

std::string GetPacketData(const ENetEvent & evnt)
{
	std::string out;
	for (int i = 0; i < evnt.packet->dataLength; ++i) {
		out.push_back(evnt.packet->data[i]);
	}

	return out;
}


PacketType FindType(string data)
{
	int i = stoi(data.substr(0, data.find_first_of(':')));

	return PacketType(i);
}

Player * Placeholder_GetPlayer(Colour id)
{
	return playerOne;
}