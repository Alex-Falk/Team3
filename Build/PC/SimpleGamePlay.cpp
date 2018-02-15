#include "WeaponPickup.h"
#include "SimpleGamePlay.h"

//--------------------------------------------------------------------------------------------//
// Initialisation and Cleanup
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::OnInitializeScene() {


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

	pickup = new Pickup(Vector3(0, 3, 0), SPEED_BOOST);

	this->AddGameObject(pickup->GetObj());

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

	if (pickup)
	{
		pickup->Update(dt);
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


//--------------------------------------------------------------------------------------------//
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}