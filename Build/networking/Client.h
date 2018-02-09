//Alexander Falk
// 05/02/2018
// Handles Client functionality for the game. Includes Sending and Recieving updates to/from the server


#pragma once

#include "User.h"
#define CLIENT_ID serverConnection->outgoingPeerID

using namespace std;

class Client : public User
{
public:
	Client();
	Client(IP ip);
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

	void RecieveSizes(string data);
	void RecieveScores(string data);
	void RecieveMapIndex(string data);
	void RecieveMapChange(string data);

protected:
	NetworkBase network;
	ENetPeer* serverConnection;

};

