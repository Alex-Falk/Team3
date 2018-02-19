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

	GraphicsPipeline::Instance()->InitPath(Vector2((float)xDimension, (float)yDimension));

	// Adding Scene Specific objects ----------------------------------------------------------------------------------------------
	GameObject* ground = CommonUtils::BuildCuboidObject(
		"Ground",
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3((float)xDimension, 1.0f, (float)yDimension),
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
	score = new Score(xDimension,yDimension, groundScoreAccuracy);
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
	score->UpdateScores();
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
// Utility. TODO: can probably (re)move this
//--------------------------------------------------------------------------------------------//
void SimpleGamePlay::onButtonClicked()
{
	SceneManager::Instance()->JumpToScene();
}