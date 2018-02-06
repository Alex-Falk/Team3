#pragma once

#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>
#include <PC\GamePlay.h>
#include "NetworkCommon.h"

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

