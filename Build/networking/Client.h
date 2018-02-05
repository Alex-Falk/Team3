#pragma once

#include "Server.h"

#define CLIENT_ID serverConnection->outgoingPeerID

class Client
{
public:
	Client();
	~Client();

	

	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//
	void UpdateClient(float dt);

	PacketType FindType(ENetEvent evnt);
	void ProcessNetworkEvent(const ENetEvent & evnt);

	//--------------------------------------------------------------------------------------------//
	// Sending
	//--------------------------------------------------------------------------------------------//

	void SendAcceleration();

	//--------------------------------------------------------------------------------------------//
	// Recieving
	//--------------------------------------------------------------------------------------------//

	void ReceivePositions(const ENetEvent & evnt);
	void RecieveAcceleration(const ENetEvent & evnt);
	void RecieveSizes(const ENetEvent & evnt);
	void RecieveScores(const ENetEvent & evnt);
	void RecieveMapIndex(const ENetEvent & evnt);
	void RecieveMapChange(const ENetEvent & evnt);

protected:
	NetworkBase network;
	ENetPeer* serverConnection;

};

