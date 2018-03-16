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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Client.h
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
// Handles Client functionality for the game. Includes Sending and Recieving updates to/from the server

#pragma once

#include "User.h"

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
	void UpdateUser(float dt);

	void Disconnect();

	void ProcessNetworkEvent(const ENetEvent & evnt);

	//--------------------------------------------------------------------------------------------//
	// Sending
	//--------------------------------------------------------------------------------------------//

	void SendUsername();
	void SendAvatarUpdate(uint ID, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc, float life, int inAir);
	void SendWeaponFire(uint ID, WeaponType type, Vector3 pos, Vector3 dir);
	void RequestPickup(uint ID, uint objectID);

	virtual void UpdateName() { SendUsername(); };

	//--------------------------------------------------------------------------------------------//
	// Recieving
	//--------------------------------------------------------------------------------------------//

	void ReceiveNumberUsers(string data);
	void ReceiveUserNames(string data);
	void ReceiveObjectUpdate(string data);
	void ReceiveScores(string data);
	void ReceiveMapIndex(string data);
	void ReceiveRequestResponse(string data, PhysNodeType ptype);
	void ReceiveMinionSpawn(string data);
	void ReceiveMinionUpdate(string data);
	void ReceiveMinionDeath(string data);
	void ReceiveAreaCapture(string data);

	void DeadReckonObject(GameObject * go, TempObjData data, float dt);
protected:
	//--------------------------------------------------------------------------------------------//
	// Stored Variables
	//--------------------------------------------------------------------------------------------//
	NetworkBase network;
	ENetPeer* serverConnection;
	float accumTime = 0.0f;


};

