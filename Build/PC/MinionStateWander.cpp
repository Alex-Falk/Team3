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
														88  888                  WanderState                 888  88
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
#include "MinionStates.h"

MinionStateWander* MinionStateWander::instance;

MinionStateWander::MinionStateWander()
{
}

MinionStateWander* MinionStateWander::GetInstance()
{
	if (!instance)
	{
		instance = new MinionStateWander;
		instance->stateName = "Wander";
	}

	return instance;
}

void MinionStateWander::Enter(Minion* pMinion)
{
	pMinion->GetMinionBlackBoard()->SetWander(true);
}

std::string MinionStateWander::GetState()
{
	return stateName;
}

void MinionStateWander::Exit(Minion* pMinion)
{
	pMinion->GetMinionBlackBoard()->WipeBoard();
}

void MinionStateWander::Execute(Minion* pMinion)
{
	if (pMinion->IsAlive())
	{
		if (pMinion->IncomingProjectile())
		{
			pMinion->ChangeState(MinionStateEscapeRocket::GetInstance());
		}
		else if (pMinion->DistanceToClosestFriendlySQ() <= pMinion->GetDetectionRadiusSQ())
		{
			pMinion->ChangeState(MinionStateHealFriendlyPlayer::GetInstance());
		}
		else if (pMinion->DistanceToClosestEnemySQ() <= pMinion->GetDetectionRadiusSQ() && 
			    ((pMinion->DistanceToClosestEnemySQ() < pMinion->DistanceToEnemyZoneSQ() && pMinion->DistanceToEnemyZoneSQ() != NO_ENEMY_ZONES_FLAG) || pMinion->DistanceToEnemyZoneSQ() == NO_ENEMY_ZONES_FLAG))
		{
			pMinion->ChangeState(MinionStateChaseEnemyPlayer::GetInstance());
		}
		else if (pMinion->DistanceToEnemyZoneSQ() != NO_ENEMY_ZONES_FLAG)
		{
			pMinion->ChangeState(MinionStateCaptureZone::GetInstance());
		}
	}
}

void MinionStateWander::Release()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

MinionStateWander::~MinionStateWander()
{

}
