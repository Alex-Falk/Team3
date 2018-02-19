#include "Map.h"

//--------------------------------------------------------------------------------------------//
// Initialization
//--------------------------------------------------------------------------------------------//
void Map::onConnectToScene()
{
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); i++) {
		if (Game::Instance()->GetUser())
		{
			Avatar * p = nullptr;
			if (i == Game::Instance()->getUserID())
			{
				p = new ControllableAvatar(spawnPositions[i], Colour(i), i, 1.0f);
			}
			else
			{
				p = new Avatar(spawnPositions[i], Colour(i), i, 1.0f);
			}

			this->AddGameObject(p->GetGameObject());
			Game::Instance()->SetAvatar(i, p);

			GraphicsPipeline::Instance()->AddPlayerRenderNode(Game::Instance()->GetPlayer(i)->GetGameObject()->Render());
		}
	}
}

void Map::OnInitializeScene() {
	GraphicsPipeline::Instance()->SetIsMainMenu(false);
	Scene::OnInitializeScene();
}

void Map::OnInitializeGUI()
{
	//Create Push Button handle
	energyBar = static_cast<CEGUI::ProgressBar*>(
		GUIsystem::Instance()->createWidget("TaharezLook/ProgressBar",
			Vector4(0.40f, 0.9f, 0.2f, 0.03f),
			Vector4(),
			"energyBar"
		));

	if (Game::Instance()->GetUser())
	{
		if (Game::Instance()->GetPlayer(Game::Instance()->getUserID()))
			energyBar->setProgress(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetLife() / 100.0f);
	}

}


//--------------------------------------------------------------------------------------------//
// Updating Avatars and Scores
//--------------------------------------------------------------------------------------------//
void Map::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	//player->OnPlayerUpdate(dt);
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); i++) {
		if (Game::Instance()->GetPlayer(i))
			Game::Instance()->GetPlayer(i)->OnAvatarUpdate(dt);
	}

	int score = groundTeamScore[0];

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	if (Game::Instance()->GetUser())
	{
		if (Game::Instance()->GetPlayer(Game::Instance()->getUserID()))
			energyBar->setProgress(Game::Instance()->GetCurrentAvatar()->GetLife() / 100.0f);
	}
}
