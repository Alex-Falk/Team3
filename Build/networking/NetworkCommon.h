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
	AVATAR_UPDATE,
	PLAYER_WEAPON,			// Server->Client: Broadcast pos/dir		Client->Server:	Inform of spawn pos/dir
	PLAYER_SCORES,			// Server->Client: Broadcast Scores
	PLAYER_NAME,			// Client->Server: Send Username
	MAP_INDEX,				// Server->Client: Boradcast Map to load
	MAP_PICKUP_REQUEST,
	MAP_OBJECT_REQUEST,
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

