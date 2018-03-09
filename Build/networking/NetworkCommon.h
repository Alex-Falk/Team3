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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			NetworkCommon.h
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
// Common functions/enums used both by the server and the client

#pragma once
#include <nclgl\common.h>
#include <PC\Avatar.h>
//--------------------------------------------------------------------------------------------//
// Networking Enums
//--------------------------------------------------------------------------------------------//

enum PacketType {
	GAME_START,				// Informs clients the game has started
	CONNECTION_ID,
	NUMBER_USERS,
	OBJECT_UPDATE,
	PLAYER_UPDATE,
	PLAYER_WEAPON,			// Server->Client: Broadcast pos/dir		Client->Server:	Inform of spawn pos/dir
	PLAYER_SCORES,			// Server->Client: Broadcast Scores
	PLAYER_NAME,			// Client->Server: Send Username
	MAP_INDEX,				// Server->Client: Boradcast Map to load
	MAP_PICKUP_REQUEST,
	MAP_OBJECT_CAPTURE,
	MINION_SPAWN,
	MINION_UPDATE,
	MINION_DEATH
};

//--------------------------------------------------------------------------------------------//
// Useful Structs
//--------------------------------------------------------------------------------------------//

struct IP {
	int a;
	int b;
	int c;
	int d;
	int port;
};

struct PlayerVector {
	uint ID;
	Vector3 v;
};

struct PlayerFloat {
	uint ID;
	float f;
};

struct PlayerName {
	uint ID;
	std::string n;
};

//--------------------------------------------------------------------------------------------//
// Functions
//--------------------------------------------------------------------------------------------//

// Take a string in the format: "x.xx y.yy z.zz"
Vector3 InterpretStringVector(string message);

string Vector3ToString(Vector3 in);

// Function to split a string into a vector of strings by a delimiter d
vector<string> split_string(string s, char d);

PacketType FindType(string data);

Vector3 LerpVector3(Vector3 a, Vector3 b, float f);

