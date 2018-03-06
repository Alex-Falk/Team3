																			 /*ood888888888888boo.,
																	  .od888P^""            ""^Y888bo.
																  .od8P''   ..oood88888888booo.    ``Y8bo.
															   .odP'"  .ood8888888888888888888888boo.  "`Ybo.
															 .d8'   od8'd888888888f`8888't888888888b`8bo   `Yb.
															d8'  od8^   8888888888[  `'  ]8888888888   ^8bo  `8b
														  .8P  d88'     8888888888P      Y8888888888     `88b  Y8.
														 d8' .d8'       `Y88888888'      `88888888P'       `8b. `8b
														.8P .88P            """"            """"            Y88. Y8.
														88  888                 Nick Bedford                 888  88
														88  888               EscapeRocketState              888  88
														88  888.        ..       26/02/2018       ..        .888  88
														`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
														 Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
														  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
															Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
															 `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
															   `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
																  `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
																	  `^Y888bo.,            ,.od888P^'
																		   "`^^Y888888888888P^^*/ 
#include "MinionStateEscapeRocket.h"

MinionStateEscapeRocket* MinionStateEscapeRocket::instance;

MinionStateEscapeRocket::MinionStateEscapeRocket()
{
}

MinionStateEscapeRocket* MinionStateEscapeRocket::GetInstance()
{
	if (!instance)
	{
		instance = new MinionStateEscapeRocket;
		instance->stateName = "EscapeRocket";
	}

	return instance;
}

void MinionStateEscapeRocket::Enter(Minion* pMinion)
{
	pMinion->GetMinionBlackBoard()->SetFleeTarget(true);
}

std::string MinionStateEscapeRocket::GetState()
{
	return stateName;
}

void MinionStateEscapeRocket::Exit(Minion* pMinion)
{
	pMinion->GetMinionBlackBoard()->WipeBoard();
}

void MinionStateEscapeRocket::Execute(Minion* pMinion)
{
	if (pMinion->IsAlive() && !pMinion->IncomingProjectile())
	{
		pMinion->RevertState();
	}
}

void MinionStateEscapeRocket::Release()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

MinionStateEscapeRocket::~MinionStateEscapeRocket()
{

}
