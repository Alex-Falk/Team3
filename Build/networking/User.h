#pragma once
#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>
#include "NetworkCommon.h"
#include <ncltech\SceneManager.h>

class User
{
protected: 
	uint userID = 0;
	string Username[4] = { "Player1","Player2","Player3","Player4" }; //The client's name.
	string ip;
	int mapID = 3;
	bool destroy = false;
public:
	User();
	~User();

	virtual void SendPosition(uint ID) = 0;
	virtual void SendLinVelocity(uint ID) = 0;
	virtual void SendAngVelocity(uint ID) = 0;
	virtual void SendAcceleration(uint ID) = 0;
	virtual void SendWeaponFire(uint ID) = 0;
	//void SendUsername(uint ID);

	PlayerVector ReceivePosition(string data);
	PlayerVector ReceiveLinVelocity(string data);
	PlayerVector ReceiveAngVelocity(string data);
	PlayerVector ReceiveAcceleration(string data);
	PlayerFloat ReceiveSizes(string data);
	PlayerName ReceiveUserName(string data);
	void ReceiveWeapon(string data);

	string GetPacketData(const ENetEvent & evnt);
	
	inline uint GetUserID() { return userID; }
	inline string GetIP() { return ip; }

	inline string GetPlayerName(uint ID) { return Username[ID]; }
	inline void SetPlayerName(uint ID,string name) { Username[ID] = name; }

	inline void SetIP(string _ip) { ip = _ip; }

	virtual void UpdateUser(float dt) = 0;
	virtual void Disconnect() = 0;

	virtual void StartGame();

	ENetPacket* CreatePacket(string data) {
	return enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
}
};

