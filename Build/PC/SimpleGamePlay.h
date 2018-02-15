#pragma once

#include <nclgl\NCLDebug.h>
#include <ncltech\Scene.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\TextureManager.h> 
#include "GamePlay.h"
#include "Pickup.h"
#include "Avatar.h"
#include "ControllableAvatar.h"
//#include "GroundScore.h"


// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions
//



class SimpleGamePlay : public Scene
{
private:

	float m_AccumTime = 0;
	//Avatar* player;
	Pickup* pickup;
	CEGUI::ProgressBar* energyBar;
	//GroundScore* groundScore;

public:
	SimpleGamePlay(const std::string& friendly_name)
		: Scene(friendly_name) {}

	~SimpleGamePlay()
	{
		TextureManager::Instance()->RemoveAllTexture();
		delete pickup;
	}

	void OnCleanupScene();

	virtual void OnInitializeScene() override;

	void onConnectToScene() override;
	
	virtual void OnUpdateScene(float dt) override;

	virtual void OnInitializeGUI() override;

	void onButtonClicked();

	// Everything about score
	static const int groundScoreAccuracy = 100;
	static const int xOnGrid = (int)2 * (DIMENSION_X + 10)*groundScoreAccuracy; //Array cordinates for the x position of the player on the grid
	static const int yOnGrid = (int)2 * (DIMENSION_Y + 10)*groundScoreAccuracy; //Array cordinates for the y position of the player on the grid
	Colour ground[xOnGrid][yOnGrid];
	int groundTeamScore[5];
	int teamScores[5];

	void BuildGroundScore(); //Builds the array for the ground score
	void UpdateGroundScore(Avatar* player); //Updates the ground cells 
	void ChangeGridScore(Colour teamToDecrease, Colour teamToIncrease); // updates the score
	void PrintScore(int x); // debug
};