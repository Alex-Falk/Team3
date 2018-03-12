/*               
                          .,okkkd:.                          
                       .:x0KOdooxKKkl,.                      
                   .,oOKKxc'. .. .;oOX0d:.                   
                ...oKOo;. .;dO00xc.  'cxKO, ..               
            .,lk0l...  .:oxXMMMMMWOoc'  .. ,O0d:.            
         .:d0XOo;.     ;c..kMMMMMK;.;:.     'ckKKkc'.        
      'lkKKxc'  .,.        oWMMMMO.        ''  .:d0KOo;.     
     '0Wk;. .,loo:.        :NMMMMx.        ,loo:. .,oXNc     
     ,0X: .lKWMKl.         ,KMMMWo         .;kWWXx' .kNc     
     '0X; .OMMMMWXx;.      ,0MMMNl       'o0NMMMMN: .kWc     
     '0X; .k0d0NWMMW0o,..cxKWMMMMXkl,..ckNMMMWKxkK: .kWc     
     '0X; .kl  ':okKWMNKXWMMMMMMMMMMNKXWWXOdc,. ,O: .kWc     
     '0X;  ,.      .,oXMMMMMMMMMMMMMMMWk;.      .;. .kNc     
     .,;.            '0MMMMMMMMMMMMMMMWc             ';.			Alexander Falk
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Server.h
     '0X: .:,     .,lkNMMMMMMMMMMMMMMMWKo:'.    .c' .OWl     
     '0X; .ko.':okXWMW0xkXWMMMMMMMMN0xkNMWN0xl;.:O: .OWc     
     '0X; .OX0NMMMWKx;.  .:xNMMMW0l,.  'lONMMMWKKX: .kWc     
     '0X: .OMMMMNkc.       '0MMMNc       .;dKWMMMN: .kWc     
     '0N: .;xKWKc.         ;XMMMWo          'kNXkl. .OWc     
     .xNKd:. .;loc.        cNMMMMk.       .;ol;. .,lONK;     
      .'lkKKkl,. .         dWMMWM0'        .  .:d0XOo;.      
          .:d0X0d,     ,l:;OMMMMMXl;lc.    .ckKKkc'          
             .,lxc.,c'. .:d0WMMMMXkl,. .;:.'dd:.             
                  .l0XOo;. .;oooc' .'cxKKx'                  
                    .,lkKKxc'.  .;oOK0d:.                    
                        .:d0K000KKkl,.                       
                           .,cll:.                            
*/
// Handles Server functionality for the game. Includes Sending and Recieving updates to/from all clients

#pragma once
#include "User.h"
#include <queue>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")


struct UserCaptureRequest
{
	uint userID;
	PhysNodeType type;
	uint objectID;
};

using namespace std;

class Server : public User
{
private:
	NetworkBase * server;
	GameTimer timer;
	uint serverPort = 1234;
	vector<string> clientIPAddress;

	vector<uint> connectedIDs;
	vector<uint> freeIDs;

public:
	Server();
	~Server() {
		server->Release();
		system("pause");
		exit(0);
	};


	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//

	NetworkBase * getBase() { return server; }

	void StartGame(uint mapID = 0);
	void UpdateUser(float dt);
	void Disconnect();
	void RequestPickup(uint ID, uint objectID);
	void HandleRequests();
	std::string GetPacketData(const ENetEvent & evnt)
	{
		std::string out;
		for (int i = 0; i < evnt.packet->dataLength; ++i) {
			out.push_back(evnt.packet->data[i]);
		}

		return out;
	}

	//--------------------------------------------------------------------------------------------//
	// Receiving
	//--------------------------------------------------------------------------------------------//

	void ReceiveRequest(string data, PhysNodeType physType);

	//--------------------------------------------------------------------------------------------//
	// Sending / Broadcasting
	//--------------------------------------------------------------------------------------------//

	void SendNumberUsers(uint n);
	void SendConnectionID(uint ID);
	void SendPlayerNames();
	void SendGameStart(uint mapID);
	void SendAvatarUpdate(uint ID, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc,float life, int inAir);
	void SendObjectUpdate(GameObject * go);
	void SendMinionSpawn(uint minionID, Colour c, Vector3 pos);
	void SendMinionUpdate(uint minionID, Colour c, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc, float life);
	void SendMinionDeath(uint minionID);
	void SendScores();
	void SendMap();
	void SendAreaCapture(uint ID, Colour c);
	void SendWeaponFire(uint ID,WeaponType type, Vector3 pos, Vector3 dir);

protected:
	//--------------------------------------------------------------------------------------------//
	// Stored Variables
	//--------------------------------------------------------------------------------------------//
	std::queue<UserCaptureRequest> requests;

	float accumTime = 0.0f;
};

