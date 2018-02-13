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
#include <PC/Game.h>
#define CLIENT_ID evnt.peer->incomingPeerID

void Win32_PrintAllAdapterIPAddresses()
{
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
				cIpAddress = cIpAddress->Next;
			}
			cAdapter = cAdapter->Next;
		}

		free(pAdapters);
	}

}

Server::Server() {

	server = new NetworkBase();
	printf("Server Initiated\n");

	server->Initialize(1234, 32);
	Win32_PrintAllAdapterIPAddresses();
	userID = 0;
	timer.GetTimedMS();
}

void Server::UpdateUser(float dt)
{
	server->ServiceNetwork(dt, [&](const ENetEvent& evnt) 
	{
		switch (evnt.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
		{
			printf(" - New Client Connected\n");

			// Send over information to new client
		}
		break;

		case ENET_EVENT_TYPE_RECEIVE:
		{
			string data = GetPacketData(evnt);
			PacketType type = FindType(data);

			switch (type) {
			case PLAYER_POS:
			{
				Game::Instance()->SetPosition(CLIENT_ID, ReceivePosition(data).v);
				break;
			}
			case PLAYER_LINVEL:
			{
				Game::Instance()->SetLinearVelocity(CLIENT_ID, ReceiveLinVelocity(data).v);
				break;
			}
			case PLAYER_ANGVEL:
			{
				Game::Instance()->SetAngularVelocity(CLIENT_ID, ReceiveAngVelocity(data).v);
				break;
			}
			case PLAYER_ACCELERATION:
			{
				Game::Instance()->SetAcceleration(CLIENT_ID, ReceiveAcceleration(data).v);
				break;
			}
			case TEXT_PACKET:
			{
				cout << data.substr(data.find_first_of(':') + 1) + "\n";
			}
			}
			break;
		}
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			printf(" - Client %d has disconnected.\n", CLIENT_ID);
			break;
		}
		}
	});

	Game::Instance()->SetScore(0, Game::Instance()->GetScore(0) + 1);

	for (uint i = 0; i < 4; ++i)
	{
		SendPosition(i);
		SendLinVelocity(i);
		SendAngVelocity(i);
		SendAcceleration(i);
		SendSize(i);
		SendScores();
	}

}

//--------------------------------------------------------------------------------------------//
// Recieving
//--------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------//
// Sending
//--------------------------------------------------------------------------------------------//

void Server::SendPosition(uint ID) 
{
	string data;

	data = to_string(PLAYER_POS) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->GetGameObject()->Physics()->GetPosition());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendLinVelocity(uint ID)
{
	string data;

	data = to_string(PLAYER_LINVEL) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->GetGameObject()->Physics()->GetLinearVelocity());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAngVelocity(uint ID)
{
	string data;

	data = to_string(PLAYER_ANGVEL) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->GetGameObject()->Physics()->GetAngularVelocity());

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

void Server::SendAcceleration(uint ID)
{
	string data;

	data = to_string(PLAYER_ACCELERATION) + ":" +
		to_string(ID) + ";" + Vector3ToString(Game::Instance()->GetPlayer(ID)->GetGameObject()->Physics()->GetAcceleration());

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

	for (uint i = 0; i < 4; ++i)
	{
		data = data + to_string(Game::Instance()->GetScore(i)) + " ";
	}

	ENetPacket* packet = CreatePacket(data);
	enet_host_broadcast(server->m_pNetwork, 0, packet);
}

//TODO WEAPONS stuff

//PACKET_TYPE:WEAPON_TYPE;XPOS YPOS ZPOS, XDIR YDIR ZDIR
void Server::SendWeaponFire(uint ID) 
{
	string data;

	data = to_string(PLAYER_WEAPON) + ":";
}


