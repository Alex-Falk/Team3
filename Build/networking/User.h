#pragma once
#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <PC\GamePlay.h>
#include <ncltech\NetworkBase.h>
#include "NetworkCommon.h"

class User
{
protected: 
	uint userID;

public:
	User();
	~User();

	virtual void SendPosition(uint ID) = 0;
	virtual void SendLinVelocity(uint ID) = 0;
	virtual void SendAngVelocity(uint ID) = 0;
	virtual void SendAcceleration(uint ID) = 0;
	virtual void SendWeaponFire(uint ID) = 0;

	PlayerVector ReceivePosition(string data);
	PlayerVector ReceiveLinVelocity(string data);
	PlayerVector ReceiveAngVelocity(string data);
	PlayerVector ReceiveAcceleration(string data);
	void ReceiveWeapon(string data);

	string GetPacketData(const ENetEvent & evnt);

	void updateUser(float dt);

	ENetPacket* CreatePacket(string data) {
	return enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
}
};

