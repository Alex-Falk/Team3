#include <ncltech\CommonUtils.h>
#include "Pickup.h"
#include "CaptureArea.h"
#include "Map.h"
#include "MinionBase.h"

Map::~Map() 
{
	TextureManager::Instance()->RemoveAllTexture();
	for (auto itr = pickups.begin(); itr != pickups.end(); ++itr)
	{
		(*itr)->SetToDestroy();
	}
	pickups.clear();

	for (auto itr = captureAreas.begin(); itr != captureAreas.end(); ++itr)
	{
		(*itr)->SetToDestroy();
	}
	captureAreas.clear();

	for (int i = 0; i < maxMinions; ++i)
	{
		minions[i]->SetToDestroy();
		minions[i] = nullptr;
	}
	captureAreas.clear();
};

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

			GraphicsPipeline::Instance()->AddPlayerRenderNode(Game::Instance()->GetPlayer(i)->GetGameObject()->Render()->GetChild());
		}
	}
	
	GraphicsPipeline::Instance()->ResetPath();
}

void Map::OnInitializeScene() {
	OnInitializeGUI();

	GraphicsPipeline::Instance()->InitPath(Vector2(dimensions));

	if (pickups.size() > 0)
	{
		pickups.clear();
	}

	if (captureAreas.size() > 0)
	{
		captureAreas.clear();
	}


	OnInitializeGUI();

	SetSpawnLocations();

	LoadTextures();

	AddObjects();

	PhysicsEngine::Instance()->ResetWorldPartition();

	Scene::OnInitializeScene();
}

//--------------------------------------------------------------------------------------------//
// Updating CaptureAreas Score
//--------------------------------------------------------------------------------------------//
void Map::UpdateCaptureAreas() {
	
	int captAreaTeamScore[4] = { (0,0,0,0) };
	for (uint i = 0;i < ncapture;i++)
	{
		switch (capture[i]->GetColour())
		{
		case RED:
			captAreaTeamScore[0] += capture[i]->GetScoreValue();
			break;
		case GREEN:
			captAreaTeamScore[1] += capture[i]->GetScoreValue();
			break;
		case BLUE:
			captAreaTeamScore[2] += capture[i]->GetScoreValue();
			break;
		case PINK:
			captAreaTeamScore[3] += capture[i]->GetScoreValue();
			break;
		default:
			break;
		}
	}
	Game::Instance()->SetAreaScores(0, captAreaTeamScore[0]);
	Game::Instance()->SetAreaScores(1, captAreaTeamScore[1]);
	Game::Instance()->SetAreaScores(2, captAreaTeamScore[2]);
	Game::Instance()->SetAreaScores(3, captAreaTeamScore[3]);
}

void Map::OnInitializeGUI()
{
	GraphicsPipeline::Instance()->SetIsMainMenu(false);
	GUIsystem::Instance()->drawPlayerName = true;
	GUIsystem::Instance()->SetDrawMiniMap(true);
	lifeBar = static_cast<CEGUI::ProgressBar*>(
		GUIsystem::Instance()->createWidget("TaharezLook/ProgressBar",
			Vector4(0.40f, 0.9f, 0.25f, 0.03f),
			Vector4(),
			"lifeBar"
		));
	
	timer = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Titlebar",
			Vector4(0.45f, 0.00f, 0.10f, 0.05f),
			Vector4(),
			"Timer"
		));
	timer->setText("00:00");

	isLoading = true;
	GUIsystem::Instance()->SetLoadingScreen(LoadingScreenType::TRANSITION);
}

void Map::BuildGround(Vector2 dimensions) {
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),			// Centre Position
		Vector3(dimensions.x, 1.0f, dimensions.y),		// Scale
		true,
		0.0f,
		true,
		false,								// Dragable By User
		BIG_NODE,
		Vector4(0.6f, 0.6f, 0.6f, 1.0f),
		MATERIALTYPE::Ground);	// Colour
	this->AddGameObject(ground);

	GameObject* upWall = CommonUtils::InvisibleWall(
		"UpWall",
		Vector3(-1.0f, 80, -1.0f),
		Vector3(dimensions.x+2, 2.0f, dimensions.y+2));
	this->AddGameObject(upWall);

	GameObject* eastWall = CommonUtils::InvisibleWall(
		"EastWall",
		Vector3(0.0f, 50, dimensions.x + 5),
		Vector3(dimensions.x, 50.f, 5));
	this->AddGameObject(eastWall);
	GameObject* westWall = CommonUtils::InvisibleWall(
		"WestWall",
		Vector3(0.0f, 50, -dimensions.x - 5),
		Vector3(dimensions.x, 50.f, 5));
	this->AddGameObject(westWall);

	GameObject* northWall = CommonUtils::InvisibleWall(
		"NorthWall",
		Vector3(dimensions.x + 5, 50, 0.0f),
		Vector3(5, 50.f, dimensions.y));
	this->AddGameObject(northWall);
	GameObject* southWall = CommonUtils::InvisibleWall(
		"SouthWall",
		Vector3(-dimensions.x - 5, 50, 0.0f),
		Vector3(5, 50.f, dimensions.y));
	this->AddGameObject(southWall);
}

void Map::LoadTextures()
{
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	if (!TextureManager::Instance()->LoadCubeMap(TEXTURETYPE::Sky_Box, TEXTUREDIR"SkyBox\\skyright.jpg", TEXTUREDIR"SkyBox\\skyleft.jpg", TEXTUREDIR"SkyBox\\skytop.jpg",
		TEXTUREDIR"SkyBox\\skybottom.jpg", TEXTUREDIR"SkyBox\\skyback.jpg", TEXTUREDIR"SkyBox\\skyfront.jpg"))
		NCLERROR("Texture not loaded");

	//minimap icons
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Paint_Pool, TEXTUREDIR"PaintPool.png", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Gun_Pickup, TEXTUREDIR"guns.png", GL_REPEAT, GL_LINEAR))
		NCLERROR("Texture not loaded");
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Jump_Pickup, TEXTUREDIR"jump.png", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Speed_Pickup, TEXTUREDIR"speed.png", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Capture_Point, TEXTUREDIR"capture.png", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

}

void Map::SetSpawnLocations()
{
	spawnPositions[0] = Vector3(0, 5, 0);
	spawnPositions[1] = Vector3(35, 5, -35);
	spawnPositions[2] = Vector3(-35, 5, 35);
	spawnPositions[3] = Vector3(-35, 5, -35);
}

void Map::OnCleanupScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoveAllTexture();
	GraphicsPipeline::Instance()->RemoveAllPlayerRenderNode();
	captureAreas.clear();
};

void Map::AddPickup(Pickup * p) {
	pickups.push_back(p);
	AddGameObject(p);
}
void Map::TransferAndUpdateTimer()
{
	float t_time = Game::Instance()->GetTimeLeft();
	//string t_string = std::to_string(t_time);
	float t_min = floor(t_time / 60);
	string t_string_min = std::to_string(t_min);
	t_string_min.assign(t_string_min, 0, 1);
	float t_second = t_time - t_min * 60;
	string t_string_second = std::to_string(t_second);
	if (t_second > 10) {
		t_string_second.assign(t_string_second, 0, 2);
		timer->setText("0" + t_string_min + ":" + t_string_second);
	}
	else {
		t_string_second.assign(t_string_second, 0, 1);
		timer->setText("0" + t_string_min + ":0" + t_string_second);
	}
}

void Map::AddCaptureArea(CaptureArea * ca) {
	captureAreas.push_back(ca);
	AddGameObject(ca);
}

void Map::AddMinion(MinionBase * m)
{
	for (int i = 0; i < maxMinions; ++i)
	{
		if (!minions[i])
		{
			minions[i] = m;
			AddGameObject(m);
			break;
		}
	}	
}

void Map::AddMinion(MinionBase * m, int location)
{
	if (minions[location])
	{
		minions[location]->SetToDestroy();
	}

	minions[location] = m;
	AddGameObject(m);
}

void Map::RemoveMinion(MinionBase * m)
{
	for (int i = 0; i < maxMinions; ++i)
	{
		if (minions[i] == m)
		{
			m->SetToDestroy();
			minions[i] = nullptr;
			break;
		}
	}
}

uint Map::GetMinionID(MinionBase * m)
{
	for (int i = 0; i < maxMinions; ++i)
	{
		if (minions[i] == m)
		{
			return (uint)i;
		}
	}
	return 0;
}



//--------------------------------------------------------------------------------------------//
// Updating Avatars
//--------------------------------------------------------------------------------------------//
void Map::UpdateGUI(float dt)
{
	//player->OnPlayerUpdate(dt);
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); i++) {
		if (Game::Instance()->GetPlayer(i)) {
			Game::Instance()->GetPlayer(i)->OnAvatarUpdate(dt);
			//Update position for each players for GUI
			GUIsystem::Instance()->playerNames[i] = Game::Instance()->GetName(i);
			GUIsystem::Instance()->playersPosition[i] = Game::Instance()->GetPlayer(i)->GetPosition();
		}
	}
	perfPlayer.EndTimingSection();

	//Loading screen
	if (isLoading == true) {
		if (temp_fps > 25) {
			GUIsystem::Instance()->SetLoadingScreen(LoadingScreenType::NOT_LOADING);
			isLoading = false;
		}
		else {
			temp_fps++;
		}
	}

	if (Game::Instance()->GetUser())
	{
		if (Game::Instance()->GetPlayer(Game::Instance()->getUserID()))
			lifeBar->setProgress(Game::Instance()->GetCurrentAvatar()->GetLife() / 100.0f);
	}

	perfMapObjects.BeginTimingSection();
	for (auto itr = pickups.begin(); itr != pickups.end(); ++itr)
	{
		(*itr)->Update(dt);
	}

	for (auto itr = captureAreas.begin(); itr != captureAreas.end(); ++itr)
	{
		if (Game::Instance()->getUserID() == 0)
			(*itr)->Update(dt);
	}

	for (int i = 0; i < maxMinions; ++i)
	{
		if (minions[i])
		{
			if (!minions[i]->IsAlive())
			{
				Game::Instance()->KillMinion(minions[i]);
				RemoveMinion(minions[i]);
			}
			else
			{
				(minions[i])->Update(dt);
			}
		}	
	}

	perfMapObjects.EndTimingSection();


	//update only once a second
	if (updatePerSecond >= 1.0f) {
		UpdateCaptureAreas();
		updatePerSecond = 0.0f;
	}
}
//--------------------------------------------------------------------------------------------//
// Updating Avatars
//--------------------------------------------------------------------------------------------//
void Map::OnUpdateScene(float dt)
{
	if(Game::Instance()->getUserID() == 0)
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	TransferAndUpdateTimer();

	UpdateGUI(dt);

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();
}

//--------------------------------------------------------------------------------------------//
// Sets the MAP
//--------------------------------------------------------------------------------------------//
int Map::mapIndex;
void Map::SetMapIndex(int mapIndx)
{
	mapIndex = mapIndx;
}