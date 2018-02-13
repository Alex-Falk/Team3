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

enum PacketType {
	GAME_START,				// Informs clients the game has started
	PLAYER_POS,				// Server->Client: Broadcast positions.		Client->Server: Inform on Update
	PLAYER_LINVEL,
	PLAYER_ANGVEL,
	PLAYER_ACCELERATION,	// Server->Client: Broadcast.				Client->Server:			"
	PLAYER_SIZES,			// Server->Client: Broadcast player sizes	
	PLAYER_WEAPON,			// Server->Client: Broadcast pos/dir		Client->Server:	Inform of spawn pos/dir
	PLAYER_SCORES,			// Server->Client: Broadcast Scores
	MAP_INDEX,				// Server->Client: Boradcast Map to load
	MAP_UPDATE,				//
	TEXT_PACKET,
	GAME_END				// Server->Client: Informs clients game has ended
};

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

//--------------------------------------------------------------------------------------------//
// Functions
//--------------------------------------------------------------------------------------------//

// Take a string in the format: "x.xx y.yy z.zz"
Vector3 InterpretStringVector(string message);

string Vector3ToString(Vector3 in);

// Function to split a string into a vector of strings by a delimiter d
vector<string> split_string(string s, char d);

PacketType FindType(string data);

