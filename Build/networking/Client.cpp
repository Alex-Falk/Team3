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

#include "Client.h"
#include <ncltech\SceneManager.h>
#include <PC/Game.h>
const Vector3 status_color3 = Vector3(1.0f, 0.6f, 0.6f);

Client::Client() : serverConnection(NULL)
{
	IP connectionIP;
	connectionIP.a = 127;
	connectionIP.b = 0;
	connectionIP.c = 0;
	connectionIP.d = 1;
	connectionIP.port = 1234;

	Client((IP)connectionIP);
}

Client::Client(IP ip) {
	if (enet_initialize() == 0)
	{
		if (network.Initialize(0))
		{
			NCLDebug::Log("Network: Initialized!");
			//Attempt to connect to the server on localhost:1234
			serverConnection = network.ConnectPeer(ip.a, ip.b, ip.c, ip.d, ip.port);
			NCLDebug::Log("Network: Attempting to connect to server.");
			enet_peer_timeout(serverConnection,800,800,800);
		}
	}
}


Client::~Client()
{
	enet_peer_disconnect_now(serverConnection, 0);
	network.Release();
	serverConnection = NULL;
}

void Client::UpdateUser(float dt)
{
	//Update Network
	auto callback = std::bind(
		&Client::ProcessNetworkEvent,		// Function to call
		this,								// Associated class instance
		std::placeholders::_1);				// Where to place the first parameter
	network.ServiceNetwork(dt, callback);

	// has to be outside the callback for some reason... so the callback sets destroy to true
	if (destroy)
	{
		enet_peer_disconnect_now(serverConnection, 0);
		network.Release();
		serverConnection = NULL;
		enet_deinitialize();
		Game::Instance()->ResetGame();
		SceneManager::Instance()->JumpToScene(0);
		return;
	}

	// Send Info to server
	if (Game::Instance()->IsRunning())
	{
		Avatar * p = Game::Instance()->GetCurrentAvatar();
		SendAvatarUpdate(userID,
			p->Physics()->GetPosition(),
			p->Physics()->GetLinearVelocity(),
			p->Physics()->GetAngularVelocity(),
			p->Physics()->GetAcceleration(),
			p->IsPlayerInAir()
		);
	}
}

void Client::Disconnect()
{
	enet_peer_disconnect_now(serverConnection, 0);

}


void Client::ProcessNetworkEvent(const ENetEvent& evnt)
{
	switch (evnt.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
	{
		if (evnt.peer == serverConnection)
		{
			NCLDebug::Log(status_color3, "Network: Successfully connected to server!");

			string data = to_string(TEXT_PACKET) + ":Hellooo!";
			ENetPacket* posPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
			enet_peer_send(serverConnection, 0, posPacket);
		}
		break;
	}
	
	case ENET_EVENT_TYPE_RECEIVE:
	{
		string data = GetPacketData(evnt);
		PacketType type = FindType(data);
		switch (type) {
		case GAME_START:
		{
			uint mapID = stoi(data.substr(data.find_first_of(':') + 1));
			Game::Instance()->StartGame(mapID);
			break;
		}
		case NUMBER_USERS:
		{
			ReceiveNumberUsers(data);
			break;
		}

		case CONNECTION_ID:
		{
			userID = stoi(data.substr(data.find_first_of(':') + 1));
			NCLDebug::Log("Connection ID recieved");
			break;
		}
		case AVATAR_UPDATE:
		{
			size_t colonIdx = data.find_first_of(':');
			size_t semicolonIdx = data.find_first_of(';');

			uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

			ReceiveAvatarUpdate(data);
			if (userID != playerID)
				DeadReckon(playerID, serverConnection->roundTripTime / 2000.0f);
			break;
		}
		case PLAYER_SIZES:
		{
			PlayerFloat pfloat = ReceiveSizes(data);
			Game::Instance()->SetSize(pfloat.ID, pfloat.f);
			break;
		}
		case PLAYER_SCORES:
		{
			ReceiveScores(data);
			break;
		}
		case MAP_INDEX:
		{
			ReceiveMapIndex(data);
			break;
		}
		case MAP_UPDATE:
		{
			//ReceiveMapChange(data);
			break;
		}
		case PLAYER_WEAPON:
		{
			ReceiveWeapon(data);
			break;
		}
		case GAME_END:
		{
			//Game::Instance()->EndMatch();
			break;
		}
		}
		break;
	}
	case ENET_EVENT_TYPE_DISCONNECT:
	{
		destroy = true;
		
		break;
	}
	}
}

//--------------------------------------------------------------------------------------------//
// Recieving
//--------------------------------------------------------------------------------------------//

void Client::ReceiveNumberUsers(string data)
{

	string s = data.substr(data.find_first_of(':') + 1);

	Game::Instance()->SetPlayerNumber(stoi(s));
}

void Client::ReceiveScores(string data) 
{
	string s = data.substr(data.find_first_of(':') + 1);
	vector<string> splitData = split_string(s, ' ');	

	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); ++i)
	{
		Game::Instance()->SetScore(i, stoi(splitData[i]));
	}
}

void Client::ReceiveMapIndex(string data)
{
	string s = data.substr(data.find_first_of(':') + 1);
	uint mapIndex = stoi(s);

	//Game::Instance()->LoadLevel(mapIndex);
}

void Client::ReceiveMapChange(string data)
{
	string s = data.substr(data.find_first_of(':') + 1);
	uint mapIndex = stoi(s);

	//Game::Instance()->LoadLevel(mapIndex);
}


//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Client::SendAvatarUpdate(uint ID, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc, int inAir)
{
	string data;

	data = to_string(AVATAR_UPDATE) + ":" +
		to_string(ID) + ";" +
		Vector3ToString(pos) + "," +
		Vector3ToString(linVel) + "," +
		Vector3ToString(angVel) + "," +
		Vector3ToString(acc) + "," +
		to_string(inAir);

	ENetPacket* packet = CreatePacket(data);
	enet_peer_send(serverConnection, 0, packet);

}

void Client::SendWeaponFire(uint ID, WeaponType type, Vector3 pos, Vector3 dir)
{
	string data;

	data = to_string(PLAYER_WEAPON) + ":"
		+ to_string(ID) + ";"
		+ to_string(type) + ";"
		+ Vector3ToString(pos) + ","
		+ Vector3ToString(dir);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, packet);
}

void Client::SendSize(uint ID)
{
	string data;

	data = to_string(PLAYER_SIZES) + ":" +
		to_string(ID) + ";" + to_string(Game::Instance()->GetPlayer(ID)->GetLife());

	ENetPacket* packet = CreatePacket(data);
	enet_peer_send(serverConnection, 0, packet);
}

//Nikos Fragkas
//Date 19/02
void Client::SendUsername()
{
	string data;

	data = to_string(PLAYER_NAME) + ";" ;

	ENetPacket* packet = CreatePacket(data);
	enet_peer_send(serverConnection, 0, packet);
}