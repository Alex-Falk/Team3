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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Client.cpp
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
// Clientside of the Game

#include "Client.h"
#include <PC/Game.h>
#include <PC/MinionBase.h>
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



//--------------------------------------------------------------------------------------------//
// Utility
//--------------------------------------------------------------------------------------------//

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

	

	if (Game::Instance()->IsRunning() )
	{
		accumTime += dt;
		if (accumTime > PhysicsEngine::Instance()->GetUpdateTimestep())
		{
			accumTime = 0.0f;
			Avatar * p = Game::Instance()->GetCurrentAvatar();
			SendAvatarUpdate(userID,
				p->Physics()->GetPosition(),
				p->Physics()->GetLinearVelocity(),
				p->Physics()->GetAngularVelocity(),
				p->Physics()->GetAcceleration(),
				p->GetLife(),
				p->IsPlayerInAir()
			);
		}

	}
}

void Client::Disconnect()
{
	if (serverConnection)
		enet_peer_disconnect_now(serverConnection, 0);

}

void Client::ProcessNetworkEvent(const ENetEvent& evnt)
{
	switch (evnt.type)
	{
	// When client connects to server
	case ENET_EVENT_TYPE_CONNECT:
	{
		if (evnt.peer == serverConnection)
		{
			NCLDebug::Log(status_color3, "Network: Successfully connected to server!");
		}
		break;
	}
	// When client receives a packet from the server
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
		case NUMBER_USERS:		ReceiveNumberUsers(data);				break;
		case PLAYER_NAME:		ReceiveUserNames(data);					break;
		case CONNECTION_ID:
		{
			userID = stoi(data.substr(data.find_first_of(':') + 1));
			NCLDebug::Log("Connection ID recieved");
			SendUsername();
			break;
		}
		case PLAYER_UPDATE:
		{
			size_t colonIdx = data.find_first_of(':');
			size_t semicolonIdx = data.find_first_of(';');

			uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

			ReceiveAvatarUpdate(data);
			if (userID != playerID)
				DeadReckon(playerID, serverConnection->roundTripTime / 2000.0f);
			break;
		}
		case OBJECT_UPDATE:		ReceiveObjectUpdate(data);				break;
		case PLAYER_SCORES:		ReceiveScores(data);					break;
		case PLAYER_WEAPON:		ReceiveWeapon(data);					break;
		case MAP_INDEX:			ReceiveMapIndex(data);					break;
		case MAP_PICKUP_REQUEST:ReceiveRequestResponse(data, PICKUP);	break;
		case MAP_OBJECT_CAPTURE:ReceiveAreaCapture(data);				break;
		case MINION_SPAWN:		ReceiveMinionSpawn(data);				break;
		case MINION_UPDATE:		ReceiveMinionUpdate(data);				break;
		case MINION_DEATH:		ReceiveMinionDeath(data);				break;
		}
		break;
	}
	case ENET_EVENT_TYPE_DISCONNECT:	destroy = true;		break;
	}
}

//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Client::SendUsername()
{
	string data = to_string(PLAYER_NAME) + ":" +
		to_string(userID) + ";" +
		Game::Instance()->GetName(userID);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(serverConnection, 0, packet);
}

void Client::SendAvatarUpdate(uint ID, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc, float life, int inAir)
{
	string data;

	data = to_string(PLAYER_UPDATE) + ":" +
		to_string(ID) + ";" +
		Vector3ToString(pos) + "," +
		Vector3ToString(linVel) + "," +
		Vector3ToString(angVel) + "," +
		Vector3ToString(acc) + "," +
		to_string(life) + "," +
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

void Client::RequestPickup(uint ID, uint objectID)
{
	string data;

	data = to_string(MAP_PICKUP_REQUEST) + ":" +
		to_string(ID) + ";" +
		to_string(objectID);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(serverConnection, 0, packet);
}

//--------------------------------------------------------------------------------------------//
// Recieving
//--------------------------------------------------------------------------------------------//

void Client::ReceiveNumberUsers(string data)
{

	string s = data.substr(data.find_first_of(':') + 1);

	Game::Instance()->SetPlayerNumber(stoi(s));
}

void Client::ReceiveUserNames(string data)
{
	string s = data.substr(data.find_first_of(':') + 1);
	vector<string> splitData = split_string(s, ',');

	for (uint i = 0; i < 4; ++i)
	{
		if (userID != i)
			Game::Instance()->SetPlayerName(i, splitData[i]);
	}
	
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
}

void Client::ReceiveRequestResponse(string data,PhysNodeType ptype)
{
	Map * m = (Map*)Game::Instance()->GetMap();

	uint colonIdx = (uint)(data.find_first_of(':'));
	uint semicolonIdx = (uint)(data.find_first_of(';'));

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));
	uint objectID = stoi(data.substr(semicolonIdx + 1));

	if (ptype == PICKUP)
	{
		uint commaIdx = (uint)(data.find_first_of(','));

		bool active = stoi(data.substr(commaIdx + 1));

		if (!active)
		{
			Pickup * pickup = static_cast<Pickup*>(m->GetGameObject(objectID));
			pickup->SetActive(false);
			Game::Instance()->GetCurrentAvatar()->PickUpBuffActivated();
		}
	}
}

void Client::ReceiveObjectUpdate(string data)
{
	Map * m = (Map*)Game::Instance()->GetMap();

	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint objectID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	data = data.substr(semicolonIdx + 1);

	vector<string> splitData = split_string(data, ',');

	TempObjData temp;

	temp.pos = InterpretStringVector(splitData[0]);
	temp.linVel = InterpretStringVector(splitData[1]);
	temp.angVel = InterpretStringVector(splitData[2]);
	temp.acc = InterpretStringVector(splitData[3]);

	GameObject * go = SceneManager::Instance()->GetCurrentScene()->GetGameObject(objectID);

	DeadReckonObject(go, temp, serverConnection->roundTripTime / 2000.0f);
}

//PACKET_TYPE:MINION_ID;COLOUR,posx posy posz
void Client::ReceiveMinionSpawn(string data)
{
	Map * m = (Map*)Game::Instance()->GetMap();

	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint minionID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	vector<string> splitData = split_string(data.substr(semicolonIdx + 1), ',');

	Colour col = Colour(stoi(splitData[0]));
	Vector3 pos = InterpretStringVector(splitData[1]);

	Vector4 ColourRGB = DEFAULT_COLOUR;
 	switch (col)
	{
	case RED:
		ColourRGB = RED_COLOUR;
		break;
	case BLUE:
		ColourRGB = BLUE_COLOUR;
		break;
	case GREEN:
		ColourRGB = GREEN_COLOUR;
		break;
	case PINK:
		ColourRGB = PINK_COLOUR;
		break;
	}

	MinionBase * minion = new MinionBase(col, ColourRGB, pos);
	if (m->GetMinions()[minionID])
	{
		m->GetMinions()[minionID]->SetToDestroy();
	}
	m->AddMinion(minion, minionID);
}

// PACKET_TYPE:MINION_ID;COLOUR,posx posy posz,linvx linvy linvz,angvx angvy angvz,accx accy accz,life
void Client::ReceiveMinionUpdate(string data)
{
	Map * m = (Map*)Game::Instance()->GetMap();

	size_t colonIdx = data.find_first_of(':');
	size_t semicolonIdx = data.find_first_of(';');

	uint minionID = stoi(data.substr(colonIdx + 1, semicolonIdx));

	data = data.substr(semicolonIdx + 1);

	vector<string> splitData = split_string(data, ',');

	TempObjData temp;

	Colour col = Colour(stoi(splitData[0]));
	temp.pos = InterpretStringVector(splitData[1]);
	temp.linVel = InterpretStringVector(splitData[2]);
	temp.angVel = InterpretStringVector(splitData[3]);
	temp.acc = InterpretStringVector(splitData[4]);
	float life = stof(splitData[5]);


	MinionBase * minion = m->GetMinion(minionID);

	if (minion)
	{
		DeadReckonObject(minion, temp, serverConnection->roundTripTime / 2000.0f);
		minion->SetLife(life);
	}
}

// PACKET_TYPE:MINION_ID
void Client::ReceiveMinionDeath(string data)
{
	Map * m = (Map*)Game::Instance()->GetMap();

	uint colonIdx = (uint)(data.find_first_of(':'));

	uint minionID = stoi(data.substr(colonIdx + 1));

	if (m->GetMinion(minionID))
	{
		m->GetMinion(minionID)->SetToDestroy();
		m->RemoveMinion(m->GetMinion(minionID));
	}

}

void Client::ReceiveAreaCapture(string data)
{
	Scene * m = Game::Instance()->GetMap();

	uint colonIdx = (uint)(data.find_first_of(':'));
	uint semicolonIdx = (uint)(data.find_first_of(';'));

	uint objectID = stoi(data.substr(colonIdx + 1, semicolonIdx));
	Colour c = Colour(stoi(data.substr(semicolonIdx + 1)));

	static_cast<CaptureArea*>(m->GetGameObject(objectID))->SetColour(c);
}

void Client::DeadReckonObject(GameObject * go, 
	TempObjData data, float dt)
{
	Vector3 estimatePos =
		data.pos +
		data.linVel * dt +
		data.acc * 0.5f * dt * dt;

	Vector3 estimateLinVel =
		data.linVel +
		data.acc * dt;

	Vector3 estimateAngVel =
		data.angVel +
		data.acc * dt;

	Vector3 newPos = LerpVector3(estimatePos, 
		go->Physics()->GetPosition(), lerpFactor);
	Vector3 newLinVel = LerpVector3(estimateLinVel, 
		go->Physics()->GetLinearVelocity(), lerpFactor);
	Vector3 newAngVel = LerpVector3(estimateAngVel, 
		go->Physics()->GetAngularVelocity(), lerpFactor);
	Vector3 newAcc = LerpVector3(data.acc, 
		go->Physics()->GetAcceleration(), lerpFactor);

	go->Physics()->SetPosition(newPos);
	go->Physics()->SetLinearVelocity(newLinVel);
	go->Physics()->SetAngularVelocity(newAngVel);
	go->Physics()->SetAcceleration(newAcc);
}
