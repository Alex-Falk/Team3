#include "User.h"



User::User()
{
}


User::~User()
{
}

void User::ReceivePosition(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	string p = data.substr(semicolonIdx);
	Vector3 pos = InterpretStringVector(p);

	game->SetPosition(playerID, pos);
}


void User::RecieveAcceleration(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	string a = data.substr(semicolonIdx);
	Vector3 acceleration = InterpretStringVector(a);

	game->SetAcceleration(playerID, acceleration);
}

// PACKET_TYPE:WEAPON_TYPE;XPOS YPOS ZPOS,XDIR YDIR ZDIR
void User::RecieveWeapon(string data) {
	uint colonPos = data.find_first_of(':');
	uint semicolonPos = data.find_first_of(';');
	uint commaPos = data.find_first_of(',');

	WeaponType type = static_cast<WeaponType>(stoi(data.substr(colonPos + 1, semicolonPos)));

	Vector3 pos = InterpretStringVector(data.substr(semicolonPos + 1, commaPos));
	Vector3 dir = InterpretStringVector(data.substr(commaPos + 1));

	//TODO Call a spawn weapon function for the player
}



string User::GetPacketData(const ENetEvent & evnt)
{
	std::string out;
	for (int i = 0; i < evnt.packet->dataLength; ++i) {
		out.push_back(evnt.packet->data[i]);
	}

	return out;
}