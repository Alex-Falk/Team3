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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Server.cpp
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
// Handles Serverside of the game

#include "Server.h"
#include <PC/Game.h>
#include <PC/MinionBase.h>

string Win32_PrintAllAdapterIPAddresses()
{
	string ip;
	//Initially allocate 5KB of memory to store all adapter info
	ULONG outBufLen = 5000;

	IP_ADAPTER_INFO* pAdapters = NULL;
	DWORD status = ERROR_BUFFER_OVERFLOW;

	//Keep attempting to fit all adapter info inside our buffer, allocating more memory if needed
	// Note: Will exit after 5 failed attempts, or not enough memory. Lets pray it never comes to this!
	for (int i = 0; i < 5 && (status == ERROR_BUFFER_OVERFLOW); i++)
	{
		pAdapters = (IP_ADAPTER_INFO *)malloc(outBufLen);
		if (pAdapters != NULL) {

			//Get Network Adapter Info
			status = GetAdaptersInfo(pAdapters, &outBufLen);

			// Increase memory pool if needed
			if (status == ERROR_BUFFER_OVERFLOW) {
				free(pAdapters);
				pAdapters = NULL;
			}
			else {
				break;
			}
		}
	}


	if (pAdapters != NULL)
	{
		//Iterate through all Network Adapters, and print all IPv4 addresses associated with them to the console
		// - Adapters here are stored as a linked list termenated with a NULL next-pointer
		IP_ADAPTER_INFO* cAdapter = &pAdapters[0];
		while (cAdapter != NULL)
		{
			IP_ADDR_STRING* cIpAddress = &cAdapter->IpAddressList;
			while (cIpAddress != NULL)
			{
				//printf("\t - Listening for connections on %s:%u\n", cIpAddress->IpAddress.String, 1234);
				ip = cIpAddress->IpAddress.String;
				cIpAddress = cIpAddress->Next;
			}
			cAdapter = cAdapter->Next;
		}

		free(pAdapters);
	}
	return ip;
}

Server::Server() {

	if (enet_initialize() == 0)
	{

		server = new NetworkBase();
		//printf("Server Initiated\n");

		server->Initialize(1234, 32);
		ip = Win32_PrintAllAdapterIPAddresses();
		userID = 0;
		lerpFactor = 0.6f;
		timer.GetTimedMS();

		freeIDs = { 3,2,1 };
		Game::Instance()->SetPlayerNumber(1);

		

	}
}



//--------------------------------------------------------------------------------------------//
// Utility
//--------------------------------------------------------------------------------------------//
void Server::StartGame(uint mapID)
{
	for (uint i = 0; i < server->m_pNetwork->connectedPeers; ++i)
	{
		enet_peer_ping(&server->m_pNetwork->peers[i]);
	}

	SendGameStart(mapID);

	SceneManager::Instance()->JumpToScene(mapID);
	SceneManager::Instance()->GetCurrentScene()->onConnectToScene();
	GraphicsPipeline::Instance()->GetCamera()->SetCenter(Game::Instance()->GetPlayer(0)->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);


}

void Server::UpdateUser(float dt)
{
	if (server->m_pNetwork) {
		server->ServiceNetwork(dt, [&](const ENetEvent& evnt)
		{
			switch (evnt.type)
			{
			// When a client connects check if the game is running and there is space for them to connect
			case ENET_EVENT_TYPE_CONNECT:
			{
				//printf(" - New Client Connected\n");
				if (Game::Instance()->IsRunning())
				{
					enet_peer_disconnect(evnt.peer, 0);
					//cout << clientIPAddress[0];
				}
				else
				{
					// Send over information to new client
					if (freeIDs.size() > 0)
					{
						Game::Instance()->SetPlayerNumber((uint)(1 + server->m_pNetwork->connectedPeers));
						SendNumberUsers((uint)(1 + server->m_pNetwork->connectedPeers));

						connectedIDs.push_back(freeIDs[freeIDs.size() - 1]);
						SendConnectionID(freeIDs[freeIDs.size() - 1]);
						enet_peer_timeout(evnt.peer, 800, 800, 800);
						evnt.peer->pingInterval = 100;
						freeIDs.pop_back();
						SendPlayerNames();
					}
					else
					{
						enet_peer_disconnect(&server->m_pNetwork->peers[3], 0);
					}
				}


			}
			break;
			// Handle incoming network packets
			case ENET_EVENT_TYPE_RECEIVE:
			{
				string data = GetPacketData(evnt);
				PacketType type = FindType(data);
				switch (type) {
				case PLAYER_UPDATE:
				{
					size_t colonIdx = data.find_first_of(':');
					size_t semicolonIdx = data.find_first_of(';');

					uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

					ReceiveAvatarUpdate(data);
					DeadReckon(playerID,dt);
					break;
				}
				case PLAYER_NAME:
				{
					PlayerName pName = ReceiveUserName(data);
					Game::Instance()->SetPlayerName(pName.ID, pName.n);
					SendPlayerNames();
					break;
				}
				case PLAYER_WEAPON:
				{
					ReceiveWeapon(data);

					ENetPacket* packet = CreatePacket(data);
					enet_host_broadcast(server->m_pNetwork, 0, packet);
					break;
				}
				case MAP_PICKUP_REQUEST:	ReceiveRequest(data,PICKUP); break;
				}
				break;
			}
			// If a user disconnects, remove their avatar from the game
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				//printf(" - Client %d has disconnected.\n", evnt.peer->incomingPeerID + 1);
				for (uint i = 0; i < connectedIDs.size(); ++i)
				{
					if (connectedIDs[i] - 1 == evnt.peer->incomingPeerID)
					{
						freeIDs.push_back(connectedIDs[i]);
						connectedIDs.erase(connectedIDs.begin() + i);
					}
				}

				if (Game::Instance()->IsRunning())
				{
					GraphicsPipeline::Instance()->RemovePlayerRenderNode((Game::Instance()->GetPlayer(evnt.peer->incomingPeerID + 1))->Render()->GetChild());
					SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(Game::Instance()->GetPlayer(evnt.peer->incomingPeerID + 1));
					Game::Instance()->SetAvatar(evnt.peer->incomingPeerID + 1, nullptr);
					Game::Instance()->SetPlayerName(evnt.peer->incomingPeerID + 1, "");
					SendPlayerNames();
				}
				break;
			}
			}
		});

		// Send packets specific to game running
		if (Game::Instance()->IsRunning())
		{
			accumTime += dt;
			if (accumTime > PhysicsEngine::Instance()->GetUpdateTimestep())
			{
				accumTime = 0.0f;
				HandleRequests();
				for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); ++i)
				{
					if (Game::Instance()->GetPlayer(i))
					{
						//Send Updates 
						SendAvatarUpdate(
							i,
							Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetPosition(),
							Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetLinearVelocity(),
							Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetAngularVelocity(),
							Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetAcceleration(),
							Game::Instance()->GetPlayer(i)->GetLife(),
							Game::Instance()->GetPlayer(i)->IsPlayerInAir()
						);

					}
				}

				Map * m = static_cast<Map*>(Game::Instance()->GetMap());

				// Send moving game object update
				for (GameObject * go : m->GetConstantGameObjects())
				{
					if (go->Physics())
					{
						if (go->Physics()->GetInverseMass() > 0.01f)
						{
							SendObjectUpdate(go);
						}
					}
				}

				// Send Minion updates
				MinionBase ** minions = m->GetMinions();
				for (int i = 0; i < m->GetMaxMinions(); ++i)
				{
					if (minions[i])
					{
						SendMinionUpdate(
							i,
							minions[i]->GetColour(),
							minions[i]->Physics()->GetPosition(),
							minions[i]->Physics()->GetLinearVelocity(),
							minions[i]->Physics()->GetAngularVelocity(),
							minions[i]->Physics()->GetAcceleration(),
							minions[i]->GetLife()
						);
					}
				}

				SendScores();
			}
		}
	}

}

void Server::Disconnect()
{
	server->Release();
}

void Server::RequestPickup(uint ID, uint objectID)
{
	UserCaptureRequest r;
	r.userID = ID;
	r.objectID = objectID;
	r.type = PICKUP;

	requests.push(r);
}

void Server::HandleRequests()
{
	uint numRequests = (uint)requests.size();
	string data;


	for (uint i = 0; i < numRequests; ++i)
	{
		UserCaptureRequest r = requests.front();



		Map * m = (Map*)(SceneManager::Instance()->GetCurrentScene());

		if (r.type == PICKUP)
		{
			data = to_string(MAP_PICKUP_REQUEST) + ":" +
				to_string(r.userID) + ";" +
				to_string(r.objectID) + ",";

			Pickup * pickup = static_cast<Pickup*>(m->GetGameObject(r.objectID));

			if (pickup)
			{
				if (pickup->GetActive())
				{
					data = data + "0";
					if (userID == 0)

						pickup->SetActive(false);
				}
				else
				{
					data = data + "1";
				}

				if (r.userID != userID)
				{
					ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(&server->m_pNetwork->peers[r.userID - 1], 0, packet);
				}
			}
		}
		requests.pop();
	}
}

//--------------------------------------------------------------------------------------------//
// Receiving
//--------------------------------------------------------------------------------------------//

// PACKET_TYPE:player_ID;object_ID
void Server::ReceiveRequest(string data, PhysNodeType physType)
{

	uint colonIdx = (uint)(data.find_first_of(':'));
	uint semicolonIdx = (uint)(data.find_first_of(';'));

	uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));
	uint objectID = stoi(data.substr(semicolonIdx + 1));

	UserCaptureRequest r;
	r.userID = playerID;
	r.objectID = objectID;
	r.type = physType;

	requests.push(r);
}

//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Server::SendNumberUsers(uint n)
{
	string data;

	data = to_string(NUMBER_USERS) + ":" +
		to_string(n);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendConnectionID(uint ID)
{
	string data;

	data = to_string(CONNECTION_ID) + ":" +
		to_string(ID);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(&server->m_pNetwork->peers[ID-1], 0, packet);

}

void Server::SendPlayerNames()
{
	string data;

	data = to_string(PLAYER_NAME) + ":" +
		Game::Instance()->GetName(0) + "," +
		Game::Instance()->GetName(1) + "," +
		Game::Instance()->GetName(2) + "," +
		Game::Instance()->GetName(3);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendGameStart(uint mapID)
{
	string data;

	data = to_string(GAME_START) + ":" + to_string(mapID);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->m_pNetwork, 0, packet);

}

void Server::SendObjectUpdate(GameObject * go)
{
	string data;

	data = to_string(OBJECT_UPDATE) + ":" +
		to_string(go->GetIdx()) + ";" +
		Vector3ToString(go->Physics()->GetPosition()) + "," +
		Vector3ToString(go->Physics()->GetLinearVelocity()) + "," +
		Vector3ToString(go->Physics()->GetAngularVelocity()) + "," +
		Vector3ToString(go->Physics()->GetAcceleration());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAvatarUpdate(uint ID,Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc,float life, int inAir)
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

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), 0);
	enet_host_broadcast(server->m_pNetwork, 0, packet);

}

void Server::SendMinionSpawn(uint minionID, Colour c, Vector3 pos)
{
	string data;

	data = to_string(MINION_SPAWN) + ":" +
		to_string(minionID) + ";" +
		to_string(c) + "," +
		Vector3ToString(pos);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendMinionUpdate(uint minionID, Colour c, Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc,float life)
{
	string data;

	data = to_string(MINION_UPDATE) + ":" +
		to_string(minionID) + ";" +
		to_string(c) + "," + 
		Vector3ToString(pos) + "," +
		Vector3ToString(linVel) + "," +
		Vector3ToString(angVel) + "," +
		Vector3ToString(acc) + "," +
		to_string(life);

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);

}

void Server::SendMinionDeath(uint minionID)
{
	string data;

	data = to_string(MINION_DEATH) + ":" +
		to_string(minionID);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendScores()
{
	string data;

	data = to_string(PLAYER_SCORES) + ":";

	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); ++i)
	{
		data = data + to_string(Game::Instance()->GetScore(i)) + " ";
	}

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendMap()
{
	string data;

	data = to_string(MAP_INDEX) + ":" + to_string(mapID);

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAreaCapture(uint ID, Colour c)
{
	string data;

	data = to_string(MAP_OBJECT_CAPTURE) + ":" +
		to_string(ID) + ";" +
		to_string(c);

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}
void Server::SendWeaponFire(uint ID, WeaponType type, Vector3 pos, Vector3 dir)
{
	string data;

	data = to_string(PLAYER_WEAPON) + ":"
		+ to_string(ID) + ";"
		+ to_string(type) + ";"
		+ Vector3ToString(pos) + ","
		+ Vector3ToString(dir);

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}


