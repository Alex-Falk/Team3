// Created by J. Zhou
// Adapted by A. Falk

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
#include "Game.h"
#include "AudioSystem.h"
// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions

class MainMenu : public Scene
{
private:
	Avatar* player;
	bool ShowOptionMenu;

	CEGUI::PushButton* createButton;
	CEGUI::PushButton* startButton;
	CEGUI::PushButton* joinButton;
	CEGUI::PushButton* optionButton;
	CEGUI::PushButton* OptionMenuBack;
	CEGUI::PushButton* connectToHostButton;

	CEGUI::DefaultWindow* ipText;
	//Sound Control
	CEGUI::Slider* mastervolumeSlider;
	CEGUI::Slider* GameSoundsSlider;
	CEGUI::Slider* MusicSlider;
	CEGUI::Titlebar* mastervolumeText;
	CEGUI::Titlebar* GameSoundvolumeText;
	CEGUI::Titlebar* MusicvolumeText;

	//Camera Control
	CEGUI::Slider* CameraSensitivity;
	CEGUI::Titlebar* CameraSensitivityText;

	//Vsync Control
	CEGUI::Titlebar* VsyncText;
	CEGUI::RadioButton* enableVsync;
	CEGUI::RadioButton* disableVsync;
	CEGUI::Titlebar* background;

	//Debug Control
	CEGUI::PushButton* DebugButton;

	//Testing text box for creating game
	inputBox IpInputBox;
	inputBox userName;

	//Exit Button
	CEGUI::PushButton* exitButton;

	//Map text
	CEGUI::Titlebar* Map1Text;
	CEGUI::Titlebar* Map2Text;
	CEGUI::Titlebar* Map3Text;
	CEGUI::Titlebar* Map4Text;
	//Map radio Button
	CEGUI::RadioButton* Map1Rbutton;
	CEGUI::RadioButton* Map2Rbutton;
	CEGUI::RadioButton* Map3Rbutton;
	CEGUI::RadioButton* Map4Rbutton;

	//User chosen map
	int nextMapID = 1;
public:
	MainMenu(const std::string& friendly_name)
		: Scene(friendly_name) {}
	~MainMenu();

	//Scene functions
	void OnCleanupScene();
	virtual void OnInitializeScene();
	virtual void OnUpdateScene(float dt);
	virtual void OnInitializeGUI();

	//Setting Up functions
	void SetUpMainMenu();
	void SetUpLobby();  //Create Game menu
	void SetUpconnectionMenu();
	void SetUpOptionMenu();
	void SetUpCreateGameMenu();

	//Clicked function
	//1. MainMenu buttons
	void onOptionButtonClicked() { HideMainMenu(); ShowOptionMenu1(); }
	void onButtonClicked() { SceneManager::Instance()->JumpToScene(); }
	void onCreateGameClicked();
	void onJoinGameClicked() { HideMainMenu(); ShowConnectionMenu(); }

	//2. Option Menu buttons
	void OnOptionMenuBackClicked() { ShowMainMenu(); HideOptionMenu(); }
	void OnEnableVsyncClicked() { GraphicsPipeline::Instance()->SetVsyncEnabled(true); }
	void OnDisableVsyncClicked() { GraphicsPipeline::Instance()->SetVsyncEnabled(false); }
	void OnDebugRenderClicked() { GraphicsPipeline::Instance()->SetIsMainMenu(!GraphicsPipeline::Instance()->GetIsMainMenu()); }
	//Slider function
	void onMastervolumeChanged() { float temp = mastervolumeSlider->getCurrentValue(); AudioSystem::Instance()->SetMasterVolume(temp); }
	void onGameSoundvolumeChanged() { float temp = GameSoundsSlider->getCurrentValue(); AudioSystem::Instance()->SetGameSoundsVolume(temp); }
	void onMusicvolumeChanged() { float temp = MusicSlider->getCurrentValue(); AudioSystem::Instance()->SetGameSoundsVolume(temp); }

	//3. create game menu buttons
	void onStartGameClicked() { Game::Instance()->StartGame(nextMapID); }
	void onMap1selected();
	void onMap2selected();
	void onMap3selected();
	void onMap4selected();
	void OnUserNameClicked() {
		userName.editbox->setText("");
		GUIsystem::Instance()->SetIsTyping(true);
		GUIsystem::Instance()->currentType = "UserName";
	}

	//4. join game menu buttons
	void onConnectButtonClicked();
	void onIPinputClicked();


	//Hide/show Menu helper function
	void ShowLobbyMenuServer();
	void ShowLobbyMenuClient(){}
	void ShowMainMenu();
	void ShowConnectionMenu();
	void ShowOptionMenu1();
	void HideOptionMenu();
	void HideMainMenu();
	void HideLobby();
	void HideConnectionMenu();

	//Quit the whole program cleanly
	void Quit() {
		SceneManager::Instance()->SetExitButtonClicked(true);
	}
};