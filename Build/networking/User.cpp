#include "User.h"
#include <PC/Game.h>
User::User()
{
	userID = 0;
}


User::~User()
{
}

PlayerVector User::ReceivePosition(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	string p = data.substr(semicolonIdx + 1);
	Vector3 pos = InterpretStringVector(p);

	PlayerVector pvec;
	pvec.ID = playerID;
	pvec.v = pos;

	return pvec;
}

PlayerVector User::ReceiveLinVelocity(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	string p = data.substr(semicolonIdx + 1);
	Vector3 vel = InterpretStringVector(p);

	PlayerVector pvec;
	pvec.ID = playerID;
	pvec.v = vel;

	return pvec;
}

PlayerVector User::ReceiveAngVelocity(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	string p = data.substr(semicolonIdx + 1);
	Vector3 vel = InterpretStringVector(p);

	PlayerVector pvec;
	pvec.ID = playerID;
	pvec.v = vel;

	return pvec;
}

PlayerVector User::ReceiveAcceleration(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	string a = data.substr(semicolonIdx + 1);
	Vector3 acceleration = InterpretStringVector(a);

	PlayerVector pvec;
	pvec.ID = playerID;
	pvec.v = acceleration;

	return pvec;
}

// PACKET_TYPE:WEAPON_TYPE;XPOS YPOS ZPOS,XDIR YDIR ZDIR
void User::ReceiveWeapon(string data) {
	uint colonPos = (uint)(data.find_first_of(':'));
	uint semicolonPos = (uint)(data.find_first_of(';'));
	uint commaPos = (uint)(data.find_first_of(','));

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

PlayerFloat User::ReceiveSizes(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	string a = data.substr(semicolonIdx + 1);

	PlayerFloat pfloat;
	pfloat.ID = playerID;
	pfloat.f = stof(a);

	return pfloat;

}

void User::StartGame(int mapID)
{
	SceneManager::Instance()->JumpToScene(mapID);
	SceneManager::Instance()->GetCurrentScene()->onConnectToScene();
	GraphicsPipeline::Instance()->GetCamera()->SetCenter(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);
}