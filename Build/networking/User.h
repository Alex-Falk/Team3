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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			User.h
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
// Base Class for Server and Client with some common features used by both

#pragma once
#include <enet\enet.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>

#include <ctime>
#include "NetworkCommon.h"

struct TempPlayerData {
	Vector3 positions[4];
	Vector3 linVelocities[4];
	Vector3 angVelocities[4];
	Vector3 accelerations[4];
	float sizes[4];
};

struct TempObjData {
	Vector3 pos;
	Vector3 linVel;
	Vector3 angVel;
	Vector3 acc;
};

class User
{
protected: 
	uint userID = 0;
	string userName;
	string ip;
	int mapID = 0;
	bool destroy = false;

	TempPlayerData temps;

public:
	User();
	~User();

	//--------------------------------------------------------------------------------------------//
	// Sending
	//--------------------------------------------------------------------------------------------//

	virtual void SendAvatarUpdate(uint ID, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc, float life, int inAir) = 0;
	virtual void SendWeaponFire(uint ID, WeaponType type, Vector3 pos, Vector3 dir) = 0;

	virtual void RequestPickup(uint ID, uint objectID) {};

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

