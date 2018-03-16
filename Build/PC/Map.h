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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Map.h
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
	float updatePerSecond = 0;
	Vector3 spawnPositions[4];
	
	static int mapIndex; // Controls which map will be loaded

	
	//--------------------------------------------------------------------------------------------//
	// UI Elements in the scene
	//CEGUI::ProgressBar* lifeBar;
	CEGUI::Titlebar* timer;
	CEGUI::Titlebar* background;
	CEGUI::Titlebar* background1;
	CEGUI::PushButton* exit;
	CEGUI::PushButton* _continue;
	//--------------------------------------------------------------------------------------------//

	//--------------------------------------------------------------------------------------------//
	// Map Size
	//--------------------------------------------------------------------------------------------//
	Vector2 dimensions;
	inline void SetMapDimensions(Vector2 dimens) 
	{
		dimensions = dimens; PhysicsEngine::Instance()->SetLimits(Vector3(-dimensions.x, -20, -dimensions.y), Vector3(dimensions.x, 50, dimensions.y));
	}

	inline Vector2 GetMapDimensions() { return dimensions; }
	
	static const int maxMinions = 20;
	MinionBase * minions[maxMinions];

public:
	//--------------------------------------------------------------------------------------------//
	// Initialization
	//--------------------------------------------------------------------------------------------//
	Map(const std::string& friendly_name, const std::string& mapName) : Scene(friendly_name, mapName) {}
	virtual ~Map();

	virtual void OnCleanupScene();

	void onConnectToScene() override;
	virtual void OnInitializeScene() override;
	virtual void OnInitializeGUI() override;

	void BuildInvisibleWalls(Vector2 Dimensions);
	virtual void LoadTextures();
	virtual void AddObjects() {};
	virtual void SetSpawnLocations();


	//--------------------------------------------------------------------------------------------//
	// Utility
	//--------------------------------------------------------------------------------------------//
	static int GetMapIndex() { return mapIndex; }
	void SetMapIndex(int mapIndx);

	//phil 21/02/2018 for minimap
	inline float GetXDimension() { return dimensions.x; }
	inline float GetYDimension() { return dimensions.y; }

	inline Vector3 GetSpawnPos(Colour c) { return spawnPositions[c]; }

	//--------------------------------------------------------------------------------------------//
	// Minions - Special Gameobjects
	//--------------------------------------------------------------------------------------------//

	void AddMinion(MinionBase * m);
	void AddMinion(MinionBase * m,int location);
	void RemoveMinion(MinionBase * m);
	MinionBase * GetMinion(int i) { return minions[i]; }
	uint GetMinionID(MinionBase * m);
	MinionBase ** GetMinions() { return minions; }

	int GetMaxMinions() { return maxMinions; }


	//--------------------------------------------------------------------------------------------//
	// Updating
	//--------------------------------------------------------------------------------------------//
	virtual void OnUpdateScene(float dt) override;

	//Jeffery 06/03/2018 for timer GUI
	void TransferAndUpdateTimer();

	//Jeffery 06/03/2018 for updating playername and position for GUI rendering
	void UpdateGUI(float dt);

	//J 12/03 for pause Menu
	void showPauseMenu();
	void OnExitButtonClicked();
	void OnContinueButtonClicked();

	float temp_fps = 0;
	bool isLoading = false;
};

