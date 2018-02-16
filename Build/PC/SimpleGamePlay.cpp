#include "WeaponPickup.h"
#include "SimpleGamePlay.h"
#include "PaintPool.h"
#include "Game.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnInitializeScene() {

	spawnPositions[0] = Vector3(35, 5, 35);
	spawnPositions[1] = Vector3(35, 5, -35);
	spawnPositions[2] = Vector3(-35, 5, 35);
	spawnPositions[3] = Vector3(-35, 5, -35);

	// Loading Textures -----------------------------------------------------------------------------------------------------------
	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	if (!TextureManager::Instance()->LoadCubeMap(TEXTURETYPE::Sky_Box, TEXTUREDIR"SkyBox\\skyright.jpg", TEXTUREDIR"SkyBox\\skyleft.jpg", TEXTUREDIR"SkyBox\\skytop.jpg",
		TEXTUREDIR"SkyBox\\skybottom.jpg", TEXTUREDIR"SkyBox\\skyback.jpg", TEXTUREDIR"SkyBox\\skyfront.jpg"))
		NCLERROR("Texture not loaded");

	GraphicsPipeline::Instance()->InitPath(Vector2(xDimension, yDimension));

	// Adding Scene Specific objects ----------------------------------------------------------------------------------------------
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(xDimension, 1.0f, yDimension),
		true,
		0.0f,
		true,
		false,
		PhysNodeType::BIG_NODE,
		Vector4(0.6f, 0.6f, 0.6f, 1.0f),
		MATERIALTYPE::Ground);

	this->AddGameObject(ground);

	pickup[0] = new PaintPool(Vector3(0, 0.6f, 0), RED);

	this->AddGameObject(pickup[0]);

	pickup[1] = new WeaponPickup(Vector3(5,1,5),PAINT_SPRAY,5.0f);

	this->AddGameObject(pickup[1]);

	// Score & GUI initialisation -------------------------------------------------------------------------------------------------
	BuildGroundScore();
	OnInitializeGUI();

	// General Initialization -----------------------------------------------------------------------------------------------------
	Map::OnInitializeScene();
}

void SimpleGamePlay::OnCleanupScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoveAllTexture();
	GraphicsPipeline::Instance()->RemoveAllPlayerRenderNode();
};

//--------------------------------------------------------------------------------------------//
// Special Object udpates (e.g. Pickups)
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnUpdateScene(float dt)
{
	Map::OnUpdateScene(dt);

	for (uint i = 0; i < npickup; ++i)
	{
		if (pickup[i])
		{
			pickup[i]->Update(dt);
		}
	}

}

//--------------------------------------------------------------------------------------------//
// Score Related Functions
//--------------------------------------------------------------------------------------------//
// These two need to be in each scene because they are dependent on the array which is defined in the scene itself

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
	for (int i = 0; i < Game::Instance()->GetPlayerNumber(); i++)
	{
		groundTeamScore[i] = 0;
	}
}

void SimpleGamePlay::UpdateGroundScore(Avatar* player) {

	Vector2 playerPos = Vector2((player->GetPosition().x * groundScoreAccuracy) + (xOnGrid - (5 * groundScoreAccuracy)) / 2, (player->GetPosition().z * groundScoreAccuracy) + (yOnGrid - (5 * groundScoreAccuracy)) / 2);

	if (playerPos.x > xOnGrid / 2 || playerPos.x < -xOnGrid / 2 || playerPos.y > yOnGrid / 2 || playerPos.y < -yOnGrid / 2) {
		return;
	}

	int plGridSize = (int)(player->GetLife() * groundScoreAccuracy / 100);

	// Runs through the square arount the center and finds the circle.
	for (int i = playerPos.x - plGridSize; i <= playerPos.x; i++) {
		for (int j = playerPos.y - plGridSize; j <= playerPos.y; j++) {
			if ((i - playerPos.x)*(i - playerPos.x) + (j - playerPos.y)* (j - playerPos.y) <= plGridSize * plGridSize) {
				int xSym = playerPos.x - (i - playerPos.x);
				int ySym = playerPos.y - (j - playerPos.y);
				// Thanks to symetry we take all 4 quadrants of the circle arount the center
				if (xSym < xOnGrid - 1 && xSym > 0 && ySym < yOnGrid - 1 && ySym > 0 && i > 0 && j > 0 && i < xOnGrid -1 && j < yOnGrid - 1)
				{
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
}


//--------------------------------------------------------------------------------------------//
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}