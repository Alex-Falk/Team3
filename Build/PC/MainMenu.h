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
	userInput connectIP;
public:

	//Exit Button
	CEGUI::PushButton* exitButton;

	MainMenu(const std::string& friendly_name)
		: Scene(friendly_name) {}

	~MainMenu()
	{
		enet_deinitialize();
		TextureManager::Instance()->RemoveAllTexture();
		delete player;
	}

	void OnCleanupScene() {
		DeleteAllGameObjects();
		TextureManager::Instance()->RemoveAllTexture();
		GraphicsPipeline::Instance()->RemoveAllPlayerRenderNode();
	}

	virtual void OnInitializeScene() {
		GraphicsPipeline::Instance()->SetIsMainMenu(true);

		if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
			NCLERROR("Texture not loaded");
		if (!TextureManager::Instance()->LoadCubeMap(TEXTURETYPE::Sky_Box, TEXTUREDIR"SkyBox\\skyright.jpg", TEXTUREDIR"SkyBox\\skyleft.jpg", TEXTUREDIR"SkyBox\\skytop.jpg",
			TEXTUREDIR"SkyBox\\skybottom.jpg", TEXTUREDIR"SkyBox\\skyback.jpg", TEXTUREDIR"SkyBox\\skyfront.jpg"))
			NCLERROR("Texture not loaded");

		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(40.0f, 1.0f, 40.0f),
			true,
			0.0f,
			true,
			false,
			PhysNodeType::BIG_NODE,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),
			MATERIALTYPE::Ground);

		this->AddGameObject(ground);

		player = new Avatar(Vector3(0.0, 1.0, 0.0), START_COLOUR, 0, 1.0f);

		this->AddGameObject(player->GetGameObject());

		GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
		GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(10);


		OnInitializeGUI();
		Scene::OnInitializeScene();
	}

	virtual void OnUpdateScene(float dt) {
		float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
		yaw += 0.1;
		GraphicsPipeline::Instance()->GetCamera()->SetYaw(yaw);
		Scene::OnUpdateScene(dt);
	}

	virtual void OnInitializeGUI() {
		//Call initi-function for gui
		sceneGUI = new GUI();
		sceneGUI->Init(CEGUIDIR);

		//Load Scheme - Which actually means UI style - notice that multiple Scheme could be load at once
		sceneGUI->LoadScheme("WindowsLook.scheme");
		sceneGUI->LoadScheme("TaharezLook.scheme");
		sceneGUI->LoadScheme("AlfiskoSkin.scheme");
		sceneGUI->LoadScheme("OgreTray.scheme");

		//Set Font sytle
		sceneGUI->SetFont("DejaVuSans-10");

		//SetMouseCursor
		sceneGUI->SetMouseCursor("AlfiskoSkin/MouseArrow");
		sceneGUI->ShowMouseCursor();

		SetUpMainMenu();
		SetUpOptionMenu();
		SetUpLobby();
		SetUpconnectionMenu();
		HideLobby();
		HideConnectionMenu();
		HideOptionMenu();
		
	}

	void onOptionButtonClicked(){HideMainMenu();ShowOptionMenu1();}

	void OnOptionMenuBackClicked(){ShowMainMenu();HideOptionMenu();}
	void onMastervolumeChanged(){float temp = mastervolumeSlider->getCurrentValue();AudioSystem::Instance()->SetMasterVolume(temp);}
	void onGameSoundvolumeChanged(){float temp = GameSoundsSlider->getCurrentValue();AudioSystem::Instance()->SetGameSoundsVolume(temp);}
	void onMusicvolumeChanged(){float temp = MusicSlider->getCurrentValue();AudioSystem::Instance()->SetGameSoundsVolume(temp);}
	//GUI interation event
	void onButtonClicked() { SceneManager::Instance()->JumpToScene(); }
	void OnEnableVsyncClicked() {GraphicsPipeline::Instance()->SetVsyncEnabled(true);}
	void OnDisableVsyncClicked() {GraphicsPipeline::Instance()->SetVsyncEnabled(false);}
	void OnDebugRenderClicked() {GraphicsPipeline::Instance()->SetIsMainMenu(!GraphicsPipeline::Instance()->GetIsMainMenu());}

	//GUI setting up helper function
	// 1. Setting up Main Menu
	void SetUpMainMenu()
	{
		//Create Push Button handle
		createButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.15f, 0.2f, 0.1f),
				Vector4(),
				"createButton"
			));
		createButton->setText("CREATE GAME");
		createButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onCreateGameClicked, this));

		joinButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.35f, 0.2f, 0.1f),
				Vector4(),
				"joinButton"
			));
		joinButton->setText("JOIN GAME");
		joinButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onJoinGameClicked, this));

		optionButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.55f, 0.2f, 0.1f),
				Vector4(),
				"optionButton"
			));
		optionButton->setText("OPTION");
		optionButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onOptionButtonClicked, this));


		exitButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.75f, 0.2f, 0.1f),
				Vector4(),
				"exitButton"
			));
		exitButton->setText(" EXIT ");
		exitButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::Quit, this));
	}

	void SetUpLobby()
	{
		startButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.75f, 0.2f, 0.1f),
				Vector4(),
				"startButton"
			));
		startButton->setText("START GAME");
		startButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onStartGameClicked, this));

		ipText = static_cast<CEGUI::DefaultWindow*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.1f, 0.2f, 0.1f),
				Vector4(),
				"text"
			));

	}

	void SetUpconnectionMenu()
	{
		IpInputBox.editbox = static_cast<CEGUI::Editbox*>(
			sceneGUI->createWidget("OgreTray/Editbox",
				Vector4(0.40f, 0.65f, 0.2f, 0.1f),
				Vector4(),
				"textBox"
			));
		IpInputBox.editbox->setText("127.0.0.1:1234");
		IpInputBox.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnTextBoxClicked, this));
		IpInputBox.type = "test";
		connectIP.type = IpInputBox.type;
		sceneGUI->inputBox.push_back(IpInputBox);
		sceneGUI->userTyping.push_back(connectIP);

		connectToHostButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.75f, 0.2f, 0.1f),
				Vector4(),
				"connectToHostButton"
			));
		connectToHostButton->setText("CONNECT TO GAME");
		connectToHostButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onConnectButtonClicked, this));
	}

	// 2. Setting up option Menu
	void SetUpOptionMenu()
	{
		OptionMenuBack = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.85f, 0.2f, 0.1f),
				Vector4(),
				"OptionMenuBack"
			));
		OptionMenuBack->setText("Back");
		OptionMenuBack->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnOptionMenuBackClicked, this));


		//Sound volume Control
		mastervolumeSlider = static_cast<CEGUI::Slider*>(
			sceneGUI->createWidget("OgreTray/Slider",
				Vector4(0.40f, 0.15f, 0.30f, 0.03f),
				Vector4(),
				"mastervolumeSlider"
			));
		mastervolumeSlider->setMaxValue(1.0f);
		mastervolumeSlider->setCurrentValue(AudioSystem::Instance()->GetMasterVolume());
		mastervolumeSlider->setVisible(false);
		mastervolumeSlider->disable();
		mastervolumeSlider->setCurrentValue(AudioSystem::Instance()->GetMasterVolume());
		mastervolumeSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&MainMenu::onMastervolumeChanged, this));

		GameSoundsSlider = static_cast<CEGUI::Slider*>(
			sceneGUI->createWidget("OgreTray/Slider",
				Vector4(0.40f, 0.25f, 0.30f, 0.03f),
				Vector4(),
				"GameSoundsSlider"
			));

		GameSoundsSlider->setMaxValue(1.0f);
		GameSoundsSlider->setCurrentValue(AudioSystem::Instance()->GetMasterVolume());
		GameSoundsSlider->setVisible(false);
		GameSoundsSlider->disable();
		GameSoundsSlider->setCurrentValue(AudioSystem::Instance()->GetMasterVolume());
		GameSoundsSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&MainMenu::onGameSoundvolumeChanged, this));
		MusicSlider = static_cast<CEGUI::Slider*>(
			sceneGUI->createWidget("OgreTray/Slider",
				Vector4(0.40f, 0.35f, 0.30f, 0.03f),
				Vector4(),
				"MusicSlider"
			));
		MusicSlider->setMaxValue(1.0f);
		MusicSlider->setCurrentValue(AudioSystem::Instance()->GetMasterVolume());
		MusicSlider->setVisible(false);
		MusicSlider->disable();
		MusicSlider->setCurrentValue(AudioSystem::Instance()->GetMasterVolume());
		MusicSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&MainMenu::onMusicvolumeChanged, this));

		mastervolumeText = static_cast<CEGUI::Titlebar*>(
			sceneGUI->createWidget("OgreTray/Title",
				Vector4(0.28f, 0.145f, 0.12f, 0.04f),
				Vector4(),
				"mastervolumeText"
			));
		mastervolumeText->setText("Master volume");

		GameSoundvolumeText = static_cast<CEGUI::Titlebar*>(
			sceneGUI->createWidget("OgreTray/Title",
				Vector4(0.28f, 0.245f, 0.12f, 0.04f),
				Vector4(),
				"GameSoundvolumeText"
			));
		GameSoundvolumeText->setText("Game Sound volume");

		MusicvolumeText = static_cast<CEGUI::Titlebar*>(
			sceneGUI->createWidget("OgreTray/Title",
				Vector4(0.28f, 0.345f, 0.12f, 0.04f),
				Vector4(),
				"MusicvolumeText"
			));
		MusicvolumeText->setText("Music volume");

		//Camera Sensitivity Control
		//TODO: link with the actual camera sensitivity
		CameraSensitivity = static_cast<CEGUI::Slider*>(
			sceneGUI->createWidget("OgreTray/Slider",
				Vector4(0.40f, 0.45f, 0.30f, 0.03f),
				Vector4(),
				"CameraSensitivity"
			));
		CameraSensitivity->setMaxValue(100.0f);
		CameraSensitivity->setCurrentValue(100.0f);

		CameraSensitivityText = static_cast<CEGUI::Titlebar*>(
			sceneGUI->createWidget("OgreTray/Title",
				Vector4(0.28f, 0.445f, 0.12f, 0.04f),
				Vector4(),
				"CameraSensitivityText"
			));
		CameraSensitivityText->setText("Camera Sensitivity");
		CameraSensitivityText->deactivate();
		CameraSensitivityText->setVisible(false);

		background = static_cast<CEGUI::Titlebar*>(
			sceneGUI->createWidget("OgreTray/Title",
				Vector4(0.40f, 0.535f, 0.24f, 0.07f),
				Vector4(),
				"VsyncBackground"
			));
		background->setText("Enable                   Disable");

		VsyncText = static_cast<CEGUI::Titlebar*>(
			sceneGUI->createWidget("OgreTray/Title",
				Vector4(0.28f, 0.545f, 0.12f, 0.04f),
				Vector4(),
				"VsyncText"
			));
		VsyncText->setText("Vsync Setting");
		VsyncText->disable();

		enableVsync = static_cast<CEGUI::RadioButton*>(
			sceneGUI->createWidget("OgreTray/RadioButton",
				Vector4(0.45f, 0.545f, 0.08f, 0.05f),
				Vector4(),
				"EnableVsync"
			));
		enableVsync->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnEnableVsyncClicked, this));
		enableVsync->setSelected(true);

		disableVsync = static_cast<CEGUI::RadioButton*>(
			sceneGUI->createWidget("OgreTray/RadioButton",
				Vector4(0.55f, 0.545f, 0.08f, 0.05f),
				Vector4(),
				"DisableVsync"
			));
		disableVsync->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnDisableVsyncClicked, this));

		DebugButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.65f, 0.2f, 0.1f),
				Vector4(),
				"debugButton"
			));
		DebugButton->setText("Debug Rendering");
		DebugButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnDebugRenderClicked, this));
	}

	void onCreateGameClicked()
	{
		if (!Game::Instance()->GetUser())
		{
			Game::Instance()->SetServer();
			ipText->setText("Your IP: \n" + Game::Instance()->GetUser()->GetIP());
			HideMainMenu();
			ShowLobbyMenuServer();
			createButton->disable();
		}


	}

	void onStartGameClicked() {	

		Game::Instance()->StartGame();
	}

	void onJoinGameClicked() {
		HideMainMenu();
		ShowConnectionMenu();
	}

	void onConnectButtonClicked()
	{
		IP ip;
		string IPstring(IpInputBox.editbox->getText().c_str());
		
		if (IPstring.find_first_of('.') != string::npos)
		{
			ip.a = stoi(IPstring.substr(0, IPstring.find_first_of('.')));
			IPstring = IPstring.substr(IPstring.find_first_of('.') + 1);
		}
		if (IPstring.find_first_of('.') != string::npos)
		{
			ip.b = stoi(IPstring.substr(0, IPstring.find_first_of('.')));
			IPstring = IPstring.substr(IPstring.find_first_of('.') + 1);
		}
		if (IPstring.find_first_of('.') != string::npos)
		{
			ip.c = stoi(IPstring.substr(0, IPstring.find_first_of('.')));
			IPstring = IPstring.substr(IPstring.find_first_of('.') + 1);
		}
		if (IPstring.find_first_of(':') != string::npos)
		{
			ip.d = stoi(IPstring.substr(0, IPstring.find_first_of(':')));
			IPstring = IPstring.substr(IPstring.find_first_of(':') + 1);
		}
		if (IPstring.size() > 0)
		{
			ip.port = stoi(IPstring.substr(0));

			if (!Game::Instance()->GetUser())
			{
				Game::Instance()->setClient(ip);
				HideConnectionMenu();
				ShowLobbyMenuClient();
			}


		}
	}

	// 3. Setting up Create Game Function
	void SetUpCreateGameMenu()
	{
		inputBox temp;
		userInput temp1;
		temp.editbox = static_cast<CEGUI::Editbox*>(
			sceneGUI->createWidget("OgreTray/Editbox",
				Vector4(0.40f, 0.65f, 0.2f, 0.1f),
				Vector4(),
				"textBox"
			));
		temp.editbox->setText("Test");
		temp.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnTextBoxClicked, this));
		temp.type = "test";
		temp1.type = temp.type;
		sceneGUI->inputBox.push_back(temp);
		sceneGUI->userTyping.push_back(temp1);
	}

	void OnTextBoxClicked() {
		sceneGUI->SetIsTyping(true);
		sceneGUI->currentType = "test";
	}

	//GUI Menu switch helper function
	void ShowLobbyMenuServer()
	{
		ipText->enable();
		ipText->setVisible(true);

		startButton->enable();
		startButton->setVisible(true);
	}

	void ShowLobbyMenuClient()
	{

	}

	void ShowMainMenu()
	{
		//Enable Main Menu
		createButton->enable();
		createButton->setVisible(true);
		exitButton->enable();
		exitButton->setVisible(true);
		optionButton->enable();
		optionButton->setVisible(true);
		joinButton->setVisible(true);
		joinButton->enable();
	}

	void ShowConnectionMenu()
	{
		connectToHostButton->enable();
		connectToHostButton->setVisible(true);

		IpInputBox.editbox->enable();
		IpInputBox.editbox->setVisible(true);
		
	}

	void HideOptionMenu()
	{
		//Disable option menu
		OptionMenuBack->disable();
		OptionMenuBack->setVisible(false);
		mastervolumeSlider->disable();
		mastervolumeSlider->setVisible(false);
		GameSoundsSlider->disable();
		GameSoundsSlider->setVisible(false);
		MusicSlider->disable();
		MusicSlider->setVisible(false);

		mastervolumeText->setVisible(false);
		mastervolumeText->deactivate();
		GameSoundvolumeText->setVisible(false);
		GameSoundvolumeText->deactivate();
		MusicvolumeText->setVisible(false);
		MusicvolumeText->deactivate();

		CameraSensitivity->deactivate();
		CameraSensitivity->setVisible(false);
		CameraSensitivityText->deactivate();
		CameraSensitivityText->setVisible(false);

		VsyncText->deactivate();
		VsyncText->setVisible(false);
		enableVsync->deactivate();
		enableVsync->setVisible(false);
		disableVsync->deactivate();
		disableVsync->setVisible(false);
		background->setVisible(false);

		DebugButton->disable();
		DebugButton->setVisible(false);
	}

	void HideMainMenu()
	{
		float temp = mastervolumeSlider->getCurrentValue();
		AudioSystem::Instance()->SetMasterVolume(temp);
		//Disable MainMenu
		createButton->setVisible(false);
		createButton->disable();
		exitButton->setVisible(false);
		exitButton->disable();
		optionButton->setVisible(false);
		optionButton->disable();
		joinButton->setVisible(false);
		joinButton->disable();
	}

	void HideLobby()
	{
		ipText->setVisible(false);
		ipText->disable();

		startButton->setVisible(false);
		startButton->disable();
	}

	void HideConnectionMenu()
	{
		connectToHostButton->setVisible(false);
		connectToHostButton->disable();

		IpInputBox.editbox->setVisible(false);
		IpInputBox.editbox->disable();
	}

	void ShowOptionMenu1()
	{
		float temp = GameSoundsSlider->getCurrentValue();
		AudioSystem::Instance()->SetGameSoundsVolume(temp);
	}

	//Quit the whole program cleanly
	void Quit() {
		SceneManager::Instance()->SetExitButtonClicked(true);
	}
};