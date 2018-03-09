#pragma once
#include <enet\enet.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>

#include <ctime>
#include "NetworkCommon.h"

struct TempData {
	Vector3 positions[4];
	Vector3 linVelocities[4];
	Vector3 angVelocities[4];
	Vector3 accelerations[4];
	float sizes[4];
};

class User
{
protected: 
	uint userID = 0;
	string userName;
	string ip;
	int mapID = 0;
	bool destroy = false;

	TempData temps;

public:
	User();
	~User();

	//--------------------------------------------------------------------------------------------//
	// Sending
	//--------------------------------------------------------------------------------------------//

	virtual void SendAvatarUpdate(uint ID, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc, float life, int inAir) = 0;
	virtual void SendWeaponFire(uint ID, WeaponType type, Vector3 pos, Vector3 dir) = 0;

	virtual void RequestPickup(uint ID, string uniqueName) {};
	virtual void RequestCaptureArea(uint ID, string uniqueName) {};

	//--------------------------------------------------------------------------------------------//
	// Receiving
	//--------------------------------------------------------------------------------------------//

	void ReceiveAvatarUpdate(string data);

	PlayerVector ReceiveVector(string data);
	PlayerFloat ReceiveSizes(string data);

	PlayerName ReceiveUserName(string data);
	void ReceiveWeapon(string data);


	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//
	string GetPacketData(const ENetEvent & evnt);
	
	inline uint GetUserID() { return userID; }
	inline string GetIP() { return ip; }

	inline virtual void UpdateName() {};

	inline void SetIP(string _ip) { ip = _ip; }

	virtual void UpdateUser(float dt) = 0;
	virtual void Disconnect() {};

	virtual void StartGame(uint mapID = 0);

	ENetPacket* CreatePacket(string data) {
		return enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	}

	void DeadReckon(uint ID, float dt);
	float lerpFactor = 0.5f;

};

