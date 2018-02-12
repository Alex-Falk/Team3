//Alexander Falk
// 05/02/2018
// Handles Client functionality for the game. Includes Sending and Recieving updates to/from the server

#pragma once

#include "User.h"
#define CLIENT_ID serverConnection->outgoingPeerID

using namespace std;

struct TempData {
	Vector3 positions[4];
	Vector3 linVelocities[4];
	Vector3 angVelocities[4];
	Vector3 accelerations[4];
};

class Client : public User
{
public:
	Client();
	//Client(IP ip);
	~Client();

	

	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//
	void UpdateClient(float dt);

	void ProcessNetworkEvent(const ENetEvent & evnt);

	//--------------------------------------------------------------------------------------------//
	// Sending
	//--------------------------------------------------------------------------------------------//

	void SendPosition(uint ID);
	void SendLinVelocity(uint ID);
	void SendAngVelocity(uint ID);
	void SendAcceleration(uint ID);
	void SendWeaponFire(uint ID);

	//--------------------------------------------------------------------------------------------//
	// Recieving
	//--------------------------------------------------------------------------------------------//

	void ReceiveSizes(string data);
	void ReceiveScores(string data);
	void ReceiveMapIndex(string data);
	//void ReceiveMapChange(string data);

protected:
	NetworkBase network;
	ENetPeer* serverConnection;

	TempData temps;

};

