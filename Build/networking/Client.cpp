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
	if (network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");
		//Attempt to connect to the server on localhost:1234
		serverConnection = network.ConnectPeer(127, 0, 0, 1, 1234);
		NCLDebug::Log("Network: Attempting to connect to server.");

	}
}

Client::Client(IP ip) {
	if (network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");
		//Attempt to connect to the server on localhost:1234
		serverConnection = network.ConnectPeer(ip.a, ip.b, ip.c, ip.d, ip.port);
		NCLDebug::Log("Network: Attempting to connect to server.");
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

	if (userID != 0) {
		SendPosition(userID);
		SendLinVelocity(userID);
		SendAngVelocity(userID);
		SendAcceleration(userID);
		SendSize(userID);

		for (uint i = 0; i < 4; ++i)
		{
			if (i != userID)
			{
				Game::Instance()->SetPosition(i, temps.positions[i]);
				Game::Instance()->SetLinearVelocity(i, temps.linVelocities[i]);
				Game::Instance()->SetAngularVelocity(i, temps.angVelocities[i]);
				Game::Instance()->SetAcceleration(i, temps.accelerations[i]);
				Game::Instance()->SetSize(i, temps.sizes[i]);
			}

		}
	}
}

void Client::Disconnect()
{
	enet_peer_disconnect_now(serverConnection, 0);
	//network.Release();
	//serverConnection = NULL;
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
			//Game::Instance()->InitializeMatch();
			break;
		}
		case CONNECTION_ID:
		{
			userID = stoi(data.substr(data.find_first_of(':') + 1));
			NCLDebug::Log("Connection ID recieved");

			SceneManager::Instance()->JumpToScene();
			SceneManager::Instance()->GetCurrentScene()->onConnectToScene();
			GraphicsPipeline::Instance()->GetCamera()->SetCenter(Game::Instance()->GetPlayer(userID)->GetGameObject()->Physics());
			GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);
			break;
		}
		case PLAYER_POS:
		{
			PlayerVector pvec = ReceivePosition(data);
			if (pvec.ID != userID)
				temps.positions[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_LINVEL:
		{
			PlayerVector pvec = ReceiveLinVelocity(data);
			if (pvec.ID != userID)
				temps.linVelocities[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_ANGVEL:
		{
			PlayerVector pvec = ReceiveAngVelocity(data);
			if (pvec.ID != userID)
				temps.angVelocities[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_ACCELERATION:
		{
			PlayerVector pvec = ReceiveAcceleration(data);
			if (pvec.ID != userID)
				temps.accelerations[pvec.ID] = pvec.v;
			break;
		}
		case PLAYER_SIZES:
		{
			PlayerFloat pfloat = ReceiveSizes(data);
			if (pfloat.ID != userID)
				temps.sizes[pfloat.ID] = pfloat.f;
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
		break;
	}

	}
}

//--------------------------------------------------------------------------------------------//
// Recieving
//--------------------------------------------------------------------------------------------//

void Client::ReceiveScores(string data) 
{

	string s = data.substr(data.find_first_of(':') + 1);
	vector<string> splitData = split_string(s, ' ');

	for (uint i = 0; i < 4; ++i)
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


//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Client::SendPosition(uint ID)
{
	Vector3 pos = Game::Instance()->GetPlayer(userID)->GetGameObject()->Physics()->GetPosition();

	string data = to_string(PLAYER_POS) + ":" 
		+ to_string(ID) + ";" 
		+ Vector3ToString(pos);

	ENetPacket* posPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, posPacket);
}

void Client::SendLinVelocity(uint ID)
{
	Vector3 vel = Game::Instance()->GetPlayer(userID)->GetGameObject()->Physics()->GetLinearVelocity();

	string data = to_string(PLAYER_LINVEL) + ":" 
		+ to_string(ID) + ";"
		+ Vector3ToString(vel);

	ENetPacket* velPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, velPacket);
}

void Client::SendAngVelocity(uint ID)
{
	Vector3 vel = Game::Instance()->GetPlayer(userID)->GetGameObject()->Physics()->GetAngularVelocity();

	string data = to_string(PLAYER_ANGVEL) + ":" 
		+ to_string(ID) + ";" 
		+ Vector3ToString(vel);

	ENetPacket* velPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, velPacket);
}

void Client::SendAcceleration(uint ID)
{
	Vector3 acc = Game::Instance()->GetPlayer(userID)->GetGameObject()->Physics()->GetAcceleration();

	string data = to_string(PLAYER_ACCELERATION) + ":" 
		+ to_string(ID) + ";" 
		+ Vector3ToString(acc);

	ENetPacket* accPacket = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_peer_send(serverConnection, 0, accPacket);
}

void Client::SendWeaponFire(uint ID)
{

}

void Client::SendSize(uint ID)
{
	string data;

	data = to_string(PLAYER_SIZES) + ":" +
		to_string(ID) + ";" + to_string(Game::Instance()->GetPlayer(ID)->GetLife());

	ENetPacket* packet = CreatePacket(data);
	enet_peer_send(serverConnection, 0, packet);
}