#pragma once
#include <ncltech\CommonUtils.h>
#include <ncltech\TextureManager.h> 
#include <ncltech\SceneManager.h>
#include "CaptureArea.h"
#include "Pickup.h"
#include "GamePlay.h"
#include "Game.h"

class MinionBase;


class Map : public Scene
{
protected:
	float m_AccumTime = 0;
	Vector3 spawnPositions[4];
	
	static int mapIndex; // Controls which map will be loaded

	
	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	//--------------------------------------------------------------------------------------------//
	CEGUI::ProgressBar* lifeBar;

	//--------------------------------------------------------------------------------------------//
	// Map Size
	//--------------------------------------------------------------------------------------------//
	Vector2 dimensions;
	inline void SetMapDimensions(Vector2 dimens) { dimensions = dimens; }
	inline Vector2 GetMapDimensions() { return dimensions; }


	//pickup stuff
	vector<Pickup*> pickups;
	//capture areas
	vector<CaptureArea*> captureAreas;
	
	static const int maxMinions = 20;
	MinionBase * minions[maxMinions];


public:
	//--------------------------------------------------------------------------------------------//
	// Initialization
	//--------------------------------------------------------------------------------------------//
	Map(const std::string& friendly_name) : Scene(friendly_name) {}
	virtual ~Map();

	virtual void OnCleanupScene();

	void onConnectToScene() override;
	virtual void OnInitializeScene() override;
	virtual void OnInitializeGUI() override;

	void BuildGround(Vector2 Dimensions);
	virtual void LoadTextures();
	virtual void AddObjects() {};
	virtual void SetSpawnLocations();


	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//
	static int GetMapIndex() { return mapIndex; }
	void SetMapIndex(int mapIndx);

	//phil 21/02/2018 for minimap
	inline int GetXDimension() { return dimensions.x; }
	inline int GetYDimension() { return dimensions.y; }


	//--------------------------------------------------------------------------------------------//
	// Special Objects
	//--------------------------------------------------------------------------------------------//

	//-PICKUPS-//
	void AddPickup(Pickup * p);

	Pickup * GetPickup(int i)					{ return pickups[i]; }
	vector<Pickup*> GetPickups()				{ return pickups; }

	//-CAPTURE AREAS-//
	void AddCaptureArea(CaptureArea * ca);
	CaptureArea * GetCaptureArea(int i)				{ return captureAreas[i]; }
	Colour GetCaptureAreaColour(uint i)				{ return captureAreas[i]->GetColour(); }
	Vector3 GetCaptureAreaPos(uint i)				{ return captureAreas[i]->Physics()->GetPosition(); }
	vector<CaptureArea*> GetCaptureAreaVector()		{ return captureAreas; }

	//-MINIONS-//
	void AddMinion(MinionBase * m);
	void AddMinion(MinionBase * m,int location);
	void RemoveMinion(MinionBase * m);
	MinionBase * GetMinion(int i) { return minions[i]; }
	uint GetMinionID(MinionBase * m);
	MinionBase ** GetMinions() { return minions; }

	int GetMaxMinions() { return maxMinions; }
	


	//--------------------------------------------------------------------------------------------//
	// Updating Avatars
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;




};

