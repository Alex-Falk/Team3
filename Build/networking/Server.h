/*               
                          .,okkd:.                          
                       .:x0KOdoxKKkl,.                      
                   .,oOKKxc'. . .;oOX0d:.                   
                ...oKOo;. .;dO0xc.  'cxKO, ..               
            .,lk0l...  .:oxXMMMMWOoc'  .. ,O0d:.            
         .:d0XOo;.     ;c..kMMMMK;.;:.     'ckKKkc'.        
      'lkKKxc'  .,.        oWMMMO.        ''  .:d0KOo;.     
     '0Wk;. .,loo:.        :NMMMx.        ,loo:. .,oXNc     
     ,0X: .lKWMKl.         ,KMMWo         .;kWWXx' .kNc     
     '0X; .OMMMMWXx;.      ,0MMNl       'o0NMMMMN: .kWc     
     '0X; .k0d0NWMMW0o,..cxKWMMMXkl,..ckNMMMWKxkK: .kWc     
     '0X; .kl  ':okKWMNKXWMMMMMMMMMNKXWWXOdc,. ,O: .kWc     
     '0X;  ,.      .,oXMMMMMMMMMMMMMMWk;.      .;. .kNc     
     .,;.            '0MMMMMMMMMMMMMMWc             ';.			Alexander Falk
     .lo.            '0MMMMMMMMMMMMMMWc            .cd,			05/02/2018
     '0X: .:,     .,lkNMMMMMMMMMMMMMMWKo:'.    .c' .OWl     
     '0X; .ko.':okXWMW0xkXWMMMMMMMN0xkNMWN0xl;.:O: .OWc     
     '0X; .OX0NMMMWKx;.  .:xNMMW0l,.  'lONMMMWKKX: .kWc     
     '0X: .OMMMMNkc.       '0MMNc       .;dKWMMMN: .kWc     
     '0N: .;xKWKc.         ;XMMWo          'kNXkl. .OWc     
     .xNKd:. .;loc.        cNMMMk.       .;ol;. .,lONK;     
      .'lkKKkl,. .         dWMWM0'        .  .:d0XOo;.      
          .:d0X0d,     ,l:;OMMMMXl;lc.    .ckKKkc'          
             .,lxc.,c'. .:d0WMMMXkl,. .;:.'dd:.             
                  .l0XOo;. .;ooc' .'cxKKx'                  
                    .,lkKKxc'. .;oOK0d:.                    
                        .:d0K00KKkl,.                       
                           .,cl:.                                         
*/

//Alexander Falk
// 05/02/2018
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

	virtual void StartGame(uint mapID = 0);
	void UpdateUser(float dt);
	void Disconnect();
	virtual void RequestPickup(uint ID, uint objectID);
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
	virtual void SendAvatarUpdate(uint ID, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc,float life, int inAir);
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

