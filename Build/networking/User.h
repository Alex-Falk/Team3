#pragma once
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>
#include <PC\GamePlay.h>
#include "NetworkCommon.h"

class User
{
protected: 
	uint userID;

public:
	User();
	~User();

	virtual void sendPosition(uint ID) = 0;
	virtual void SendLinVelocity(uint ID) = 0;
	virtual void SendAngVelocity(uint ID) = 0;
	virtual void SendAcceleration(uint ID) = 0;
	virtual void SendWeaponFire(uint ID) = 0;

	virtual void ReceivePosition(string data);
	virtual void RecieveAcceleration(string data);
	virtual void RecieveWeapon(string data);

	string GetPacketData(const ENetEvent & evnt);

	void updateUser(float dt);

};

