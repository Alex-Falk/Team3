#include "WeaponPickup.h"
#include "SimpleGamePlay.h"
#include "Game.h"
#include "GamePlay.h"


void SimpleGamePlay::OnInitializeScene() {
	GraphicsPipeline::Instance()->SetIsMainMenu(false);

	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	if (!TextureManager::Instance()->LoadCubeMap(TEXTURETYPE::Sky_Box, TEXTUREDIR"SkyBox\\skyright.jpg", TEXTUREDIR"SkyBox\\skyleft.jpg", TEXTUREDIR"SkyBox\\skytop.jpg",
		TEXTUREDIR"SkyBox\\skybottom.jpg", TEXTUREDIR"SkyBox\\skyback.jpg", TEXTUREDIR"SkyBox\\skyfront.jpg"))
		NCLERROR("Texture not loaded");

	//Create Ground (..everybody loves finding some common ground)
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(DIMENSION_X, 1.0f, DIMENSION_Y),
		true,
		0.0f,
		true,
		false,
		PhysNodeType::BIG_NODE,
		Vector4(0.6f, 0.6f, 0.6f, 1.0f),
		MATERIALTYPE::Ground);

	this->AddGameObject(ground);

	//BuildGroundScore();

	//player = new Player(Vector3(0.0, 1.0, 0.0), DEFAULT_COLOUR, 0, 1.0f);

	//this->AddGameObject(player->GetGameObject());

	pickup = new Pickup(Vector3(0, 3, 0), SPEED_BOOST);

	this->AddGameObject(pickup->GetObj());

	GraphicsPipeline::Instance()->InitPath(Vector2(DIMENSION_X, DIMENSION_Y));
	BuildGroundScore();
	OnInitializeGUI();
	Scene::OnInitializeScene();


}

void SimpleGamePlay::onConnectToScene()
{
	for (uint i = 0; i < 4; i++) {
		if (Game::Instance()->GetUser())
		{
			Avatar * p = nullptr;
			if (i == Game::Instance()->getUserID())
			{
				p = new ControllableAvatar(Vector3(i * 3, 1.0, 0.0), Colour(i), i, 1.0f);
			}
			else
			{
				p = new Avatar(Vector3(i * 3, 1.0, 0.0), Colour(i), i, 1.0f);
			}

			this->AddGameObject(p->GetGameObject());
			Game::Instance()->SetAvatar(i, p);

			GraphicsPipeline::Instance()->AddPlayerRenderNode(Game::Instance()->GetPlayer(i)->GetGameObject()->Render());
		}
	}
}

void SimpleGamePlay::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	//player->OnPlayerUpdate(dt);
	for (uint i = 0; i < 4; i++) {
		if (Game::Instance()->GetPlayer(i))
			Game::Instance()->GetPlayer(i)->OnAvatarUpdate(dt);
		UpdateGroundScore(Game::Instance()->GetPlayer(i));
	}

	if (pickup)
	{
		pickup->Update(dt);
	}

	

	
	int score = groundTeamScore[0];
	
	PrintScore(score);

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	if (Game::Instance()->GetUser())
	{
		if (Game::Instance()->GetPlayer(Game::Instance()->getUserID()))
			energyBar->setProgress(Game::Instance()->GetPlayer(Game::Instance()->getUserID())->GetLife() / 100.0f);
	}
}

void SimpleGamePlay::OnCleanupScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoveAllTexture();
	GraphicsPipeline::Instance()->RemoveAllPlayerRenderNode();
};

void SimpleGamePlay::OnInitializeGUI()
{
	//Call initi-function for gui
	sceneGUI = new GUI();
	sceneGUI->Init(CEGUIDIR);

	//Load Scheme - Which actually means UI style - notice that multiple Scheme could be load at once
	sceneGUI->LoadScheme("TaharezLook.scheme");
	sceneGUI->LoadScheme("AlfiskoSkin.scheme");

	//Set Font sytle
	sceneGUI->SetFont("DejaVuSans-10");

	//SetMouseCursor
	sceneGUI->SetMouseCursor("TaharezLook/MouseArrow");
	sceneGUI->ShowMouseCursor();

	//Create Push Button handle
	energyBar = static_cast<CEGUI::ProgressBar*>(
		sceneGUI->createWidget("TaharezLook/ProgressBar",
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

void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}


void SimpleGamePlay::BuildGroundScore() {


	for (int i = 0; i < xOnGrid - 1; i++) {
		for (int j = 0; j < yOnGrid - 1; j++) {
			ground[i][j] = START_COLOUR;
		}
	}

	ground[0][0] = RED;
	ground[0][yOnGrid - 1] = GREEN;
	ground[xOnGrid - 1][0] = BLUE;
	ground[xOnGrid - 1][yOnGrid - 1] = PINK;
	for (int i = 0; i < 4; i++)
	{
		groundTeamScore[i] = 0;
	}
}

void SimpleGamePlay::UpdateGroundScore(Avatar* player) {

	Vector2 playerPos = Vector2((player->GetPosition().x * groundScoreAccuracy) + xOnGrid / 2, (player->GetPosition().z * groundScoreAccuracy) + yOnGrid / 2);
	int plGridSize = (int)(player->GetLife() * groundScoreAccuracy / 100);

	// Runs through the square arount the center and finds the circle.
	for (int i = playerPos.x - plGridSize; i <= playerPos.x; i++) {
		for (int j = playerPos.y - plGridSize; j <= playerPos.y; j++) {
			if ((i - playerPos.x)*(i - playerPos.x) + (j - playerPos.y)* (j - playerPos.y) <= plGridSize * plGridSize) {
				int xSym = playerPos.x - (i - playerPos.x);
				int ySym = playerPos.y - (i - playerPos.y);
				// Thanks to symetry we take all 4 quadrants of the circle arount the center
				ChangeGridScore(ground[i][j], player->GetColour());
				ground[i][j] = player->GetColour();
				ChangeGridScore(ground[i][ySym], player->GetColour());
				ground[i][ySym] = player->GetColour();
				ChangeGridScore(ground[xSym][j], player->GetColour());
				ground[xSym][j] = player->GetColour();
				ChangeGridScore(ground[xSym][ySym], player->GetColour());
				ground[xSym][ySym] = player->GetColour();
			}
		}
	}
}

// Decreases the score from previous team and increases the score to the new team.
void SimpleGamePlay::ChangeGridScore(Colour teamToDecrease, Colour teamToIncrease) {
	groundTeamScore[teamToDecrease] -= 1;
	groundTeamScore[teamToIncrease] += 1;
}

void SimpleGamePlay::PrintScore(int score) {
	NCLDebug::Log(to_string(score/1000));
}