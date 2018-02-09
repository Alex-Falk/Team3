#pragma once
#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>
#include <PC\GamePlay.h>
#include "NetworkCommon.h"
#define CLIENT_ID serverConnection->outgoingPeerID

using namespace std;

class Client
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

	void SendPosition();
	void SendLinVelocity();
	void SendAngVelocity();
	void SendAcceleration();

	//--------------------------------------------------------------------------------------------//
	// Recieving
	//--------------------------------------------------------------------------------------------//

	void ReceivePositions(string data);
	void RecieveAcceleration(string data);
	void RecieveSizes(string data);
	void RecieveScores(string data);
	void RecieveMapIndex(string data);
	void RecieveMapChange(string data);
	void RecieveWeapon(string data);

	std::string GetPacketData(const ENetEvent & evnt)
	{
		std::string out;
		for (int i = 0; i < evnt.packet->dataLength; ++i) {
			out.push_back(evnt.packet->data[i]);
		}

		return out;
	}

protected:
	NetworkBase network;
	ENetPeer* serverConnection;

};

