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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Map.cpp
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
// General Map Class that serves as a base for Maps - now as a base for the DataDrivenMap.
// Extended by J. Zhou and N. Fragkas
#include <ncltech\CommonUtils.h>
#include "Pickup.h"
#include "CaptureArea.h"
#include "Map.h"
#include "MinionBase.h"
#include "ControllableAvatar.h"

Map::~Map() 
{
	TextureManager::Instance()->RemoveAllTexture();

	for (int i = 0; i < maxMinions; ++i)
	{
		minions[i]->SetToDestroy();
		minions[i] = nullptr;
	}
};

//--------------------------------------------------------------------------------------------//
// Initialization
//--------------------------------------------------------------------------------------------//
// Done when the game is actually started, needs to be after initialization and after initial network setup
void Map::onConnectToScene()
{
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); i++) {
		if (Game::Instance()->GetUser())
		{
			Avatar * p = nullptr;
			if (i == Game::Instance()->GetUserID())
			{
				p = new ControllableAvatar(spawnPositions[i], Colour(i), i, 1.0f);
			}
			else
			{
				p = new Avatar(spawnPositions[i], Colour(i), i, 1.0f);
				p->SetLife(50.0f);
			}

			this->AddGameObject(p->GetGameObject(),true);
			Game::Instance()->SetAvatar(i, p);

			GraphicsPipeline::Instance()->AddPlayerRenderNode(Game::Instance()->GetPlayer(i)->GetGameObject()->Render()->GetChild());
		}
	}
	
	
	GraphicsPipeline::Instance()->ResetPath();
}

// Done when the scene is loaded
void Map::OnInitializeScene() {
	PostProcess::Instance()->SetPostProcessType(PostProcessType::SOBEL);

	SetMapDimensions(dimensions);
	
	OnInitializeGUI();

	GraphicsPipeline::Instance()->InitPath(Vector2(dimensions));

	// reset minions
	for (int i = 0; i < maxMinions; ++i)
	{
		if (minions[i])
		{
			minions[i]->SetToDestroy();
		}
		minions[i] = nullptr;
	}

	SetSpawnLocations();

	LoadTextures();

	AddObjects();
	Scene::OnInitializeScene();
	PhysicsEngine::Instance()->ResetWorldPartition();

	
}

void Map::OnInitializeGUI()
{
	GraphicsPipeline::Instance()->SetIsMainMenu(false);
	GUIsystem::Instance()->drawPlayerName = true;
	GUIsystem::Instance()->SetIsTyping(false);
	GUIsystem::Instance()->SetDrawMiniMap(true);
	GUIsystem::Instance()->SetDrawLifeBar(true);
	
	background = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Menubar",
			Vector4(0.292f, 0.04f, 0.415f, 0.04f),
			Vector4(),
			"scorebarBackground"
		));

	background1 = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Menubar",
			Vector4(0.292f, 0.917f, 0.415f, 0.04f),
			Vector4(),
			"lifebarBackground"
		));

	timer = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.45f, 0.00f, 0.10f, 0.03f),
			Vector4(),
			"Timer"
		));
	timer->setAlpha(0.7f);
	timer->setText("00:00");

	isLoading = true;
	GUIsystem::Instance()->SetLoadingScreen(LoadingScreenType::TRANSITION);

	exit = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.45f, 0.70f, 0.10f, 0.05f),
			Vector4(),
			"exit_pause"
		));
	exit->setText("EXIT");
	exit->setAlpha(0.9f);
	exit->disable();
	exit->setVisible(false);
	exit->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Map::OnExitButtonClicked, this));


	_continue = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.45f, 0.60f, 0.10f, 0.05f),
			Vector4(),
			"continue_pause"
		));
	_continue->setText("CONTINUE");
	_continue->setAlpha(0.9f);
	_continue->disable();
	_continue->setVisible(false);
	_continue->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&Map::OnContinueButtonClicked, this));
}

// Builds Invisible walls - Nikos Fragkas
void Map::BuildInvisibleWalls(Vector2 dimensions) {

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
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Ground_Texture, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
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
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Change_Color_Mask, TEXTUREDIR"mask2.png", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::WaterDUDV, TEXTUREDIR"waterDUDV.png", GL_MIRRORED_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Water, TEXTUREDIR"Water.jpg", GL_REPEAT, GL_NEAREST))
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
};

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


//--------------------------------------------------------------------------------------------//
// Minions
//--------------------------------------------------------------------------------------------//


void Map::AddMinion(MinionBase * m)
{
	for (int i = 0; i < maxMinions; ++i)
	{
		if (!minions[i])
		{
			minions[i] = m;
			AddGameObject(m,true);
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
	AddGameObject(m,true);
}

void Map::RemoveMinion(MinionBase * m)
{
	for (int i = 0; i < maxMinions; ++i)
	{
		if (minions[i] == m)
		{
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
// Updating GUI
//--------------------------------------------------------------------------------------------//
void Map::UpdateGUI(float dt)
{
	//player->OnPlayerUpdate(dt);
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); i++) {
		if (Game::Instance()->GetPlayer(i)) {
			Game::Instance()->GetPlayer(i)->Update(dt);
			//Update position for each players for GUI
			GUIsystem::Instance()->playerNames[i] = Game::Instance()->GetName(i);
			GUIsystem::Instance()->playersPosition[i] = Game::Instance()->GetPlayer(i)->GetPosition();
		}
	}

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

	//Update player life
	if (Game::Instance()->GetUser())
	{
		if (Game::Instance()->GetPlayer(Game::Instance()->GetUserID()))
			GUIsystem::Instance()->UpdateLifebar(Game::Instance()->GetCurrentAvatar()->GetLife() / 100.0f);
	}
}
void Map::showPauseMenu()
{
	exit->enable();
	exit->setVisible(true);
	_continue->enable();
	_continue->setVisible(true);
	PostProcess::Instance()->SetPostProcessType(PostProcessType::PERFORMANCE_BLUR);
	GUIsystem::Instance()->SetIsPaused(true);
}
void Map::OnExitButtonClicked()
{
	//Return to MainMenu
	Game::Instance()->ResetGame();
	SceneManager::Instance()->JumpToScene(0);
	PostProcess::Instance()->SetPostProcessType(PostProcessType::SOBEL);
	GUIsystem::Instance()->SetIsPaused(false);
}
void Map::OnContinueButtonClicked()
{
	exit->disable();
	exit->setVisible(false);
	_continue->disable();
	_continue->setVisible(false);
	PostProcess::Instance()->SetPostProcessType(PostProcessType::SOBEL);
	GUIsystem::Instance()->SetIsPaused(false);
}
//--------------------------------------------------------------------------------------------//
// Updating Scene
//--------------------------------------------------------------------------------------------//
void Map::OnUpdateScene(float dt)
{
	if(Game::Instance()->IsHost())
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	TransferAndUpdateTimer();

	UpdateGUI(dt);

	//if (m_AccumTime > PhysicsEngine::Instance()->GetUpdateTimestep())
	//{
		perfMapObjects.BeginTimingSection();
		for (int i = 0; i < this->mapConstantObjects.size(); ++i)
		{
			if (mapConstantObjects[i])
			{
				mapConstantObjects[i]->Update(dt);
			}
		}
		perfMapObjects.EndTimingSection();

		for (int i = 0; i < this->mapDynamicObjects.size(); ++i)
		{
			if (mapDynamicObjects[i])
			{
				mapDynamicObjects[i]->Update(dt);
			}
		}
	//}


	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_PAUSE)) {
		showPauseMenu();
	}
}

//--------------------------------------------------------------------------------------------//
// Sets the MAP
//--------------------------------------------------------------------------------------------//
int Map::mapIndex;
void Map::SetMapIndex(int mapIndx)
{
	mapIndex = mapIndx;
}