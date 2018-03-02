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

#include "Server.h"
#include <ncltech\SceneManager.h>
#include <PC/Game.h>

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
				printf("\t - Listening for connections on %s:%u\n", cIpAddress->IpAddress.String, 1234);
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
		printf("Server Initiated\n");

		server->Initialize(1234, 32);
		ip = Win32_PrintAllAdapterIPAddresses();
		userID = 0;
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
	SendGameStart(mapID);
	SceneManager::Instance()->JumpToScene(mapID);
	SceneManager::Instance()->GetCurrentScene()->onConnectToScene();
	GraphicsPipeline::Instance()->GetCamera()->SetCenter(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);

	for (uint i = 0; i < server->m_pNetwork->connectedPeers; ++i)
	{
		enet_peer_ping(&server->m_pNetwork->peers[i]);
	}
}

void Server::UpdateUser(float dt)
{
	if (server->m_pNetwork) {
		server->ServiceNetwork(dt, [&](const ENetEvent& evnt)
		{
			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				printf(" - New Client Connected\n");
				if (Game::Instance()->IsRunning())
				{
					enet_peer_disconnect(evnt.peer, 0);
					cout << clientIPAddress[0];
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
						enet_peer_timeout(evnt.peer, 800000, 800000, 800000);
						evnt.peer->pingInterval = 100;
						freeIDs.pop_back();
					}
					else
					{
						enet_peer_disconnect(&server->m_pNetwork->peers[3], 0);
					}
				}


			}
			break;

			case ENET_EVENT_TYPE_RECEIVE:
			{
				string data = GetPacketData(evnt);
				PacketType type = FindType(data);
				switch (type) {
				case AVATAR_UPDATE:
				{
					size_t colonIdx = data.find_first_of(':');
					size_t semicolonIdx = data.find_first_of(';');

					uint playerID = stoi(data.substr(colonIdx + 1, semicolonIdx));

					ReceiveAvatarUpdate(data);
					DeadReckon(playerID,dt);
					break;
				}
				case PLAYER_SIZES:
				{
					PlayerFloat pfloat = ReceiveSizes(data);
					Game::Instance()->SetSize(pfloat.ID, pfloat.f);
					break;
				}
				case PLAYER_NAME:
				{
					PlayerName pName = ReceiveUserName(data);
					SetPlayerName(pName.ID, pName.n);
					break;
				}
				case PLAYER_WEAPON:
				{
					ReceiveWeapon(data);

					ENetPacket* packet = CreatePacket(data);
					enet_host_broadcast(server->m_pNetwork, 0, packet);
					break;
				}
				case PLAYER_INPUT:
				{
					//ReceiveInput(data);
					break;
				}
				}
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				printf(" - Client %d has disconnected.\n", evnt.peer->incomingPeerID + 1);
				for (uint i = 0; i < connectedIDs.size(); ++i)
				{
					if (connectedIDs[i] - 1 == evnt.peer->incomingPeerID)
					{
						freeIDs.push_back(connectedIDs[i]);
						connectedIDs.erase(connectedIDs.begin() + i);
					}
				}

				GraphicsPipeline::Instance()->RemovePlayerRenderNode((Game::Instance()->GetPlayer(evnt.peer->incomingPeerID + 1))->Render()->GetChild());
				SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(Game::Instance()->GetPlayer(evnt.peer->incomingPeerID + 1));
				Game::Instance()->SetAvatar(evnt.peer->incomingPeerID + 1, nullptr);

				//Game::Instance()->SetPlayerNumber(1 + server->m_pNetwork->connectedPeers);
				//SendNumberUsers(1 + server->m_pNetwork->connectedPeers);

				break;
			}
			}
		});

		if (Game::Instance()->IsRunning())
		{
			for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); ++i)
			{
				if (Game::Instance()->GetPlayer(i))
				{

					SendAvatarUpdate(
						i,
						Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetPosition(),
						Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetLinearVelocity(),
						Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetAngularVelocity(),
						Game::Instance()->GetPlayer(i)->GetGameObject()->Physics()->GetAcceleration(),
						Game::Instance()->GetPlayer(i)->IsPlayerInAir()
					);

					SendSize(i);
					SendScores();
				}
			}
		}
	}

}

void Server::Disconnect()
{
	server->Release();
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

void Server::SendGameStart(uint mapID)
{
	string data;

	data = to_string(GAME_START) + ":" + to_string(mapID);

	ENetPacket* packet = enet_packet_create(data.c_str(), sizeof(char) * data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->m_pNetwork, 0, packet);

}

void Server::SendAvatarUpdate(uint ID,Vector3 pos, Vector3 linVel, Vector3 angVel, Vector3 acc,int inAir)
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
	enet_host_broadcast(server->m_pNetwork, 0, packet);

}

void Server::SendSize(uint ID)
{
	string data;
	
	data = to_string(PLAYER_SIZES) + ":" +
		to_string(ID) + ";" + to_string(Game::Instance()->GetPlayer(ID)->GetLife());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendScores()
{
	string data;

	data = to_string(PLAYER_SCORES) + ":";

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

void Server::SendWeaponFire(uint ID, WeaponType type, Vector3 pos, Vector3 dir)
{
	string data;

	data = to_string(PLAYER_WEAPON) + ":"
		+ to_string(ID) + ";"
		+ to_string(type) + ";"
		+ Vector3ToString(pos) + ","
		+ Vector3ToString(dir);
}
