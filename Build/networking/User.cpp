#include "User.h"
#include <PC/Game.h>
User::User()
{
	userID = 0;
}


User::~User()
{
}

// packetType:ID;posx posy posz, linvx linvy linvz, angvx angvy angvz, accx accy accz
void User::ReceiveAvatarUpdate(string data)
{
	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	data = data.substr(semicolonIdx + 1);

	Vector3 vecs[4];
	string temp;

	for (uint i = 0; i < 4; ++i)
	{
		size_t commaIdx = data.find_first_of(',');
		if (commaIdx != string::npos)
			temp = data.substr(0, commaIdx);
		else
			temp = data;

		vecs[i] = InterpretStringVector(temp);

		data = data.substr(commaIdx + 1);
	}

	int inAir = stoi(data);

	temps.positions[playerID] = vecs[0];
	temps.linVelocities[playerID] = vecs[1];
	temps.angVelocities[playerID] = vecs[2];
	temps.accelerations[playerID] = vecs[3];
}

PlayerVector User::ReceiveVector(string data)
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

// PACKET_TYPE:WEAPON_TYPE;XPOS YPOS ZPOS,XDIR YDIR ZDIR
void User::ReceiveWeapon(string data) {
	uint colonPos = (uint)(data.find_first_of(':'));
	uint firstSemicolonPos = (uint)(data.find_first_of(';'));
	uint secondSemicolonPos = firstSemicolonPos + 1 + (uint)(data.substr(firstSemicolonPos + 1).find_first_of(';'));
	uint commaPos = (uint)(data.find_first_of(','));

	uint ID = stoi(data.substr(colonPos + 1, firstSemicolonPos));

	if (ID != Game::Instance()->getUserID())
	{
		WeaponType type = static_cast<WeaponType>(stoi(data.substr(firstSemicolonPos + 1, secondSemicolonPos)));

		Vector3 pos = InterpretStringVector(data.substr(secondSemicolonPos + 1, commaPos));
		Vector3 dir = InterpretStringVector(data.substr(commaPos + 1));

		//TODO Call a spawn weapon function for the player
		Avatar * p = Game::Instance()->GetPlayer(ID);

		switch (type)
		{
		case PAINT_ROCKET:
			p->ShootRocket(pos, dir);
			break;
		case PAINT_PISTOL:
		case AUTO_PAINT_LAUNCHER:
			p->ShootProjectile(pos, dir);
			break;
		case PAINT_SPRAY:
			p->Spray(pos, dir);
			break;
		}
	}


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

PlayerName  User::ReceiveUserName(string data) {
	
	uint playerID;

	for (uint i = 0; i < 4;i++) {
		if (userName[i] == " ") { 
			playerID = i;
			break;
		}
	}
	PlayerName pname;
	pname.n = (data);
	pname.ID = playerID;

	return pname;
}

void User::StartGame(uint mapID)
{
	SceneManager::Instance()->JumpToScene(mapID);
	SceneManager::Instance()->GetCurrentScene()->onConnectToScene();
	GraphicsPipeline::Instance()->GetCamera()->SetCenter(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);
}

void User::DeadReckon(uint ID, float dt)
{

	Avatar * p = Game::Instance()->GetPlayer(ID);

	Vector3 estimatePos =
		temps.positions[ID] +
		temps.linVelocities[ID] * dt +
		temps.accelerations[ID] * 0.5f * dt * dt;

	Vector3 estimateLinVel =
		temps.linVelocities[ID] +
		temps.accelerations[ID] * dt;

	Vector3 estimateAngVel =
		temps.angVelocities[ID] +
		temps.accelerations[ID] * dt;

	Vector3 newPos = LerpVector3(estimatePos, p->Physics()->GetPosition(), lerpFactor);
	Vector3 newLinVel = LerpVector3(estimateLinVel, p->Physics()->GetLinearVelocity(), lerpFactor);
	Vector3 newAngVel = LerpVector3(estimateAngVel, p->Physics()->GetAngularVelocity(), lerpFactor);
	Vector3 newAcc = LerpVector3(temps.accelerations[ID], p->Physics()->GetAcceleration(), lerpFactor);

	p->Physics()->SetPosition(newPos);
	p->Physics()->SetLinearVelocity(newLinVel);
	p->Physics()->SetAngularVelocity(newAngVel);
	p->Physics()->SetAcceleration(newAcc);
}