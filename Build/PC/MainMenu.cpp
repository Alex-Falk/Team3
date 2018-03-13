#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
#include "AudioSystem.h"
#include "Game.h"
#include "Avatar.h"
#include "MainMenu.h"

string temp[4] = { "", "", "", "" };

MainMenu::~MainMenu()
{
	enet_deinitialize();
	TextureManager::Instance()->RemoveAllTexture();
	delete player;
}

void MainMenu::OnCleanupScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoveAllTexture();
	GraphicsPipeline::Instance()->RemoveAllPlayerRenderNode();
}

void MainMenu::OnInitializeScene()
{
	AudioSystem::Instance()->StopAllSounds();
	AudioSystem::Instance()->Update();
	AudioSystem::Instance()->PlayASound(MENU_MUSIC, true);

	GraphicsPipeline::Instance()->SetIsMainMenu(true);
	GUIsystem::Instance()->SetDrawScoreBar(false);
	GUIsystem::Instance()->SetDrawMiniMap(false);
	PostProcess::Instance()->SetPostProcessType(PostProcessType::HDR_BLOOM);

	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Ground_Texture, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
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

	player = new Avatar(Vector3(0.0, 1.0, 0.0), GREEN, 0, 1.0f);

	this->AddGameObject(player->GetGameObject());

	GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(10);


	OnInitializeGUI();
	Scene::OnInitializeScene();
}

//A simple helper function only exist in mainmenu - temp solution
void MainMenu::TextInputHelper()
{
	if (GUIsystem::Instance()->GetIsTyping() == false) {
		if (GUIsystem::Instance()->currentType == "UserName") {
			Game::Instance()->SetName(userName.editbox->getText().c_str());
			GUIsystem::Instance()->SetIsTyping(false);
		}
		else if (GUIsystem::Instance()->currentType == "ClientName") {
			Game::Instance()->SetName(clientName.editbox->getText().c_str());
			GUIsystem::Instance()->SetIsTyping(false);
		}
		else if(GUIsystem::Instance()->currentType == "customMap"){
			//TODO:set next map name here


			GUIsystem::Instance()->SetIsTyping(false);
		}
		else {
			return;
		}
		GUIsystem::Instance()->sendInfo = false;
	}
}

void MainMenu::onStartGameClicked() 
{ 
	Game::Instance()->StartGame(nextMapID); 
	GUIsystem::Instance()->SetIsTyping(false); 
}

void MainMenu::OnUpdateScene(float dt)
{
	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	yaw += 0.1f;
	GraphicsPipeline::Instance()->GetCamera()->SetYaw(yaw);
	Scene::OnUpdateScene(dt);

	//Send player name info
	if (GUIsystem::Instance()->sendInfo == true) {
		TextInputHelper();
	}
	
	for (uint i = 0; i < Game::Instance()->GetPlayerNumber(); ++i) {
		temp[i] = Game::Instance()->GetName(i);
	}

	//Update Player Info
	AllPlayerInfo->setText(playerText 
		+ temp[0] + "\n\n"
		+ temp[1] + "\n\n"
		+ temp[2] + "\n\n"
		+ temp[3] + "\n\n");

	otherPlayersInfo->setText(playerText
		+ temp[0] + "\n\n"
		+ temp[1] + "\n\n"
		+ temp[2] + "\n\n"
		+ temp[3] + "\n\n");
}

//Setting UP GUI
void MainMenu::OnInitializeGUI()
{
	SetUpMainMenu();
	SetUpOptionMenu();
	SetUpLobby();
	SetUpconnectionMenu();
	HideLobby();
	HideConnectionMenu();
	HideOptionMenu();
	HideWaitingInfo();
}

void MainMenu::SetUpMainMenu()
{
	//Create Push Button handle
	createButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.15f, 0.2f, 0.1f),
			Vector4(),
			"createButton"
		));
	createButton->setText("CREATE GAME");
	createButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onCreateGameClicked, this));

	joinButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.35f, 0.2f, 0.1f),
			Vector4(),
			"joinButton"
		));
	joinButton->setText("JOIN GAME");
	joinButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onJoinGameClicked, this));

	optionButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.55f, 0.2f, 0.1f),
			Vector4(),
			"optionButton"
		));
	optionButton->setText("OPTION");
	optionButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onOptionButtonClicked, this));


	exitButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.75f, 0.2f, 0.1f),
			Vector4(),
			"exitButton"
		));
	exitButton->setText(" EXIT ");
	exitButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::Quit, this));
}

//Actual Create Game Menu
void MainMenu::SetUpLobby()
{
	startButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.70f, 0.2f, 0.1f),
			Vector4(),
			"startButton"
		));
	startButton->setText("START GAME");
	startButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onStartGameClicked, this));

	ipText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.40f, 0.1f, 0.2f, 0.1f),
			Vector4(),
			"text"
		));

	//Map1 text
	Map1Text = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.65f, 0.10f, 0.20f, 0.1f),
			Vector4(),
			"Map1Text"
		));
	Map1Text->setText("MAP1: Dong Li's bedroom");

	Map2Text = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.65f, 0.30f, 0.20f, 0.1f),
			Vector4(),
			"Map2Text"
		));
	Map2Text->setText("MAP2: Urban Science Building");

	Map3Text = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.65f, 0.50f, 0.20f, 0.1f),
			Vector4(),
			"Map3Text"
		));
	Map3Text->setText("MAP3: 404 not found");

	Map4Text = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.65f, 0.70f, 0.20f, 0.1f),
			Vector4(),
			"Map4Text"
		));
	Map4Text->setText("MAP4: give this a fcking name");
	
	customMap.editbox = static_cast<CEGUI::Editbox*>(
		GUIsystem::Instance()->createWidget("OgreTray/Editbox",
			Vector4(0.65f, 0.85f, 0.2f, 0.05f),
			Vector4(),
			"customMap"
		));
	customMap.type = "customMap";
	customMap.editbox->setText("Type Data driven map name");
	customMap.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnCostomMapClicked, this));
	GUIsystem::Instance()->editboxes.push_back(customMap);

	userName.editbox = static_cast<CEGUI::Editbox*>(
		GUIsystem::Instance()->createWidget("OgreTray/Editbox",
			Vector4(0.40f, 0.60f, 0.2f, 0.05f),
			Vector4(),
			"userName"
		));
	userName.type = "UserName";
	userName.editbox->setText("Dong Li");
	userName.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnUserNameClicked, this));
	GUIsystem::Instance()->editboxes.push_back(userName);

	Map1Rbutton = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.85f, 0.12f, 0.08f, 0.05f),
			Vector4(),
			"Map1Rbutton"
		));
	Map1Rbutton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onMap1selected, this));
	Map1Rbutton->setSelected(true);

	Map2Rbutton = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.85f, 0.32f, 0.08f, 0.05f),
			Vector4(),
			"Map2Rbutton"
		));
	Map2Rbutton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onMap2selected, this));

	Map3Rbutton = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.85f, 0.52f, 0.08f, 0.05f),
			Vector4(),
			"Map3Rbutton"
		));
	Map3Rbutton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onMap3selected, this));

	Map4Rbutton = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.85f, 0.72f, 0.08f, 0.05f),
			Vector4(),
			"Map4Rbutton"
		));
	Map4Rbutton->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onMap4selected, this));

	AllPlayerInfo = static_cast<CEGUI::Titlebar*>(
			GUIsystem::Instance()->createWidget("OgreTray/Title",
				Vector4(0.10f, 0.10f, 0.20f, 0.30f),
				Vector4(),
				"AllPlayerInfo"
			));
	AllPlayerInfo->setAlpha(0.8f);
	AllPlayerInfo->disable();
	AllPlayerInfo->setVisible(false);

	lobbyMenuBack = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.85f, 0.2f, 0.1f),
			Vector4(),
			"lobbyMenuBack"
		));
	lobbyMenuBack->setText("BACK");
	lobbyMenuBack->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onLobbyMenuBackButtonClicked, this));

	ComfirmHostName = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.15f, 0.70f, 0.2f, 0.1f),
			Vector4(),
			"ComfirmHostName"
		));
	ComfirmHostName->setText("CONFIRM");
	ComfirmHostName->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainMenu::onHostNameConfirmed, this));
}

void MainMenu::onHostNameConfirmed()
{
	//TODO: Set Host Name
	
}

void MainMenu::HideLobby()
{
	ipText->setVisible(false);
	ipText->disable();

	startButton->setVisible(false);
	startButton->disable();

	Map1Text->setVisible(false);
	Map1Text->disable();

	Map2Text->setVisible(false);
	Map2Text->disable();

	Map3Text->setVisible(false);
	Map3Text->disable();

	Map4Text->setVisible(false);
	Map4Text->disable();

	userName.editbox->setVisible(false);
	userName.editbox->disable();

	customMap.editbox->setVisible(false);
	customMap.editbox->disable();

	Map1Rbutton->setVisible(false);
	Map1Rbutton->disable();

	Map2Rbutton->setVisible(false);
	Map2Rbutton->disable();

	Map3Rbutton->setVisible(false);
	Map3Rbutton->disable();

	Map4Rbutton->setVisible(false);
	Map4Rbutton->disable();

	AllPlayerInfo->disable();
	AllPlayerInfo->setVisible(false);

	lobbyMenuBack->disable();
	lobbyMenuBack->setVisible(false);

	ComfirmHostName->disable();
	ComfirmHostName->setVisible(false);
}

void MainMenu::onLobbyMenuBackButtonClicked()
{
	HideLobby();
	ShowMainMenu();
}
void MainMenu::onMap1selected()
{
	nextMapID = 1;
}
void MainMenu::onMap2selected()
{
	nextMapID = 2;
}
void MainMenu::onMap3selected()
{
	nextMapID = 3;
}
void MainMenu::onMap4selected()
{
	nextMapID = 4;
}

void MainMenu::ShowLobbyMenuServer()
{
	ipText->enable();
	ipText->setVisible(true);

	startButton->enable();
	startButton->setVisible(true);

	Map1Text->setVisible(true);
	Map1Text->enable();

	Map2Text->setVisible(true);
	Map2Text->enable();

	Map3Text->setVisible(true);
	Map3Text->enable();

	Map4Text->setVisible(true);
	Map4Text->enable();

	userName.editbox->setVisible(true);
	userName.editbox->enable();

	customMap.editbox->setVisible(true);
	customMap.editbox->enable();

	Map1Rbutton->setVisible(true);
	Map1Rbutton->enable();

	Map2Rbutton->setVisible(true);
	Map2Rbutton->enable();

	Map3Rbutton->setVisible(true);
	Map3Rbutton->enable();

	Map4Rbutton->setVisible(true);
	Map4Rbutton->enable();

	AllPlayerInfo->enable();
	AllPlayerInfo->setVisible(true);

	lobbyMenuBack->enable();
	lobbyMenuBack->setVisible(true);

	ComfirmHostName->enable();
	ComfirmHostName->setVisible(true);
}

void MainMenu::SetUpconnectionMenu()
{
	//Inputting IP
	IpInputBox.editbox = static_cast<CEGUI::Editbox*>(
		GUIsystem::Instance()->createWidget("OgreTray/Editbox",
			Vector4(0.40f, 0.55f, 0.2f, 0.1f),
			Vector4(),
			"IpInputBox"
		));
	IpInputBox.editbox->setText("127.0.0.1:1234");
	IpInputBox.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onIPinputClicked, this));
	IpInputBox.type = "IpInput";
	GUIsystem::Instance()->editboxes.push_back(IpInputBox);

	connectToHostButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.65f, 0.2f, 0.1f),
			Vector4(),
			"connectToHostButton"
		));
	connectToHostButton->setText("CONNECT TO GAME");
	connectToHostButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onConnectButtonClicked, this));

	disconnectToHost = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.75f, 0.2f, 0.1f),
			Vector4(),
			"disconnectToHost"
		));
	disconnectToHost->setText("DISCONNECT");
	disconnectToHost->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OndisconnectButtonClicked, this));

	otherPlayersInfo = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.40f, 0.2f, 0.20f, 0.30f),
			Vector4(),
			"otherPlayersInfo"
		));
	otherPlayersInfo->setAlpha(0.8f);
	otherPlayersInfo->disable();
	otherPlayersInfo->setVisible(false);

	//Inputting userName
	clientName.editbox = static_cast<CEGUI::Editbox*>(
		GUIsystem::Instance()->createWidget("OgreTray/Editbox",
			Vector4(0.40f, 0.55f, 0.2f, 0.1f),
			Vector4(),
			"Client Name"
		));
	clientName.editbox->setText("Dong Li");
	clientName.type = "ClientName";
	clientName.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnClientNameClicked, this));
	GUIsystem::Instance()->editboxes.push_back(clientName);

	connectMenuBack = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.80f, 0.2f, 0.1f),
			Vector4(),
			"connectMenuBack"
		));
	connectMenuBack->setText("BACK");
	connectMenuBack->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnConnectMenuBackButtonClicked, this));
}

void MainMenu::OnConnectMenuBackButtonClicked()
{
	HideConnectionMenu();
	ShowMainMenu();
}

void MainMenu::onIPinputClicked()
{
	GUIsystem::Instance()->SetIsTyping(true);
	GUIsystem::Instance()->currentType = "IpInput";
}

void MainMenu::OnClientNameClicked()
{
	clientName.editbox->setText("");
	GUIsystem::Instance()->SetIsTyping(true);
	GUIsystem::Instance()->currentType = "ClientName";
}

void MainMenu::SetUpOptionMenu()
{
	OptionMenuBack = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.85f, 0.2f, 0.1f),
			Vector4(),
			"OptionMenuBack"
		));
	OptionMenuBack->setText("Back");
	OptionMenuBack->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnOptionMenuBackClicked, this));


	//Sound volume Control
	mastervolumeSlider = static_cast<CEGUI::Slider*>(
		GUIsystem::Instance()->createWidget("OgreTray/Slider",
			Vector4(0.40f, 0.10f, 0.30f, 0.03f),
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
		GUIsystem::Instance()->createWidget("OgreTray/Slider",
			Vector4(0.40f, 0.20f, 0.30f, 0.03f),
			Vector4(),
			"GameSoundsSlider"
		));

	GameSoundsSlider->setMaxValue(1.0f);
	GameSoundsSlider->setCurrentValue(AudioSystem::Instance()->GetGameSoundsVolume());
	GameSoundsSlider->setVisible(false);
	GameSoundsSlider->disable();
	GameSoundsSlider->setCurrentValue(AudioSystem::Instance()->GetGameSoundsVolume());
	GameSoundsSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&MainMenu::onGameSoundvolumeChanged, this));
	MusicSlider = static_cast<CEGUI::Slider*>(
		GUIsystem::Instance()->createWidget("OgreTray/Slider",
			Vector4(0.40f, 0.30f, 0.30f, 0.03f),
			Vector4(),
			"MusicSlider"
		));
	MusicSlider->setMaxValue(1.0f);
	MusicSlider->setCurrentValue(AudioSystem::Instance()->GetMusicVolume());
	MusicSlider->setVisible(false);
	MusicSlider->disable();
	MusicSlider->setCurrentValue(AudioSystem::Instance()->GetMusicVolume());
	MusicSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&MainMenu::onMusicvolumeChanged, this));

	mastervolumeText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.095f, 0.12f, 0.04f),
			Vector4(),
			"mastervolumeText"
		));
	mastervolumeText->setText("Master volume");

	GameSoundvolumeText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.195f, 0.12f, 0.04f),
			Vector4(),
			"GameSoundvolumeText"
		));
	GameSoundvolumeText->setText("Game Sound volume");

	MusicvolumeText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.295f, 0.12f, 0.04f),
			Vector4(),
			"MusicvolumeText"
		));
	MusicvolumeText->setText("Music volume");

	//Camera Sensitivity Control
	//TODO: link with the actual camera sensitivity
	CameraSensitivity = static_cast<CEGUI::Slider*>(
		GUIsystem::Instance()->createWidget("OgreTray/Slider",
			Vector4(0.40f, 0.40f, 0.30f, 0.03f),
			Vector4(),
			"CameraSensitivity"
		));
	CameraSensitivity->setMaxValue(100.0f);
	CameraSensitivity->setCurrentValue(100.0f);

	CameraSensitivityText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.395f, 0.12f, 0.04f),
			Vector4(),
			"CameraSensitivityText"
		));
	CameraSensitivityText->setText("Camera Sensitivity");
	CameraSensitivityText->deactivate();
	CameraSensitivityText->setVisible(false);

	background = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.40f, 0.485f, 0.30f, 0.30f),
			Vector4(),
			"VsyncBackground"
		));
	background->setText("");
	background->disable();
	background->setVisible(false);

	VsyncText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.495f, 0.12f, 0.04f),
			Vector4(),
			"VsyncText"
		));
	VsyncText->setText("Other Settings");
	VsyncText->disable();

	enableVsync = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.42f, 0.495f, 0.11f, 0.05f),
			Vector4(),
			"EnableVsync"
		));
	enableVsync->setText("EnableVsync");
	enableVsync->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnEnableVsyncClicked, this));
	enableVsync->setSelected(true);

	disableVsync = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.55f, 0.495f, 0.11f, 0.05f),
			Vector4(),
			"DisableVsync"
		));
	disableVsync->setText("DisableVsync");
	disableVsync->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnDisableVsyncClicked, this));
	
	enableBloomButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.42f, 0.55f, 0.11f, 0.05f),
			Vector4(),
			"bloomButton"
		));
	enableBloomButton->setText("ENABLE BLOOM");
	enableBloomButton->subscribeEvent(CEGUI::Slider::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainMenu::onEnableBloomButtonClicked, this));

	disableBloomButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.55f, 0.55f, 0.11f, 0.05f),
			Vector4(),
			"disableBloomButton"
		));
	disableBloomButton->setText("DISABLE BLOOM");
	disableBloomButton->subscribeEvent(CEGUI::Slider::EventMouseButtonDown, CEGUI::Event::Subscriber(&MainMenu::onDisableBloomButtonClicked, this));
}

void MainMenu::onCameraSensitivityChanged()
{
	float temp = CameraSensitivity->getCurrentValue(); 
}

void MainMenu::onEnableBloomButtonClicked()
{
	PostProcess::Instance()->SetPostProcessType(PostProcessType::HDR_BLOOM);
}

void MainMenu::onDisableBloomButtonClicked()
{
	PostProcess::Instance()->SetPostProcessType(PostProcessType::BASIC);
}

void MainMenu::onCreateGameClicked()
{
	
	if (!Game::Instance()->GetUser())
	{
		Game::Instance()->SetServer();
		ipText->setText("Your IP: \n" + Game::Instance()->GetUser()->GetIP());
		createButton->disable();
	}
	HideMainMenu();
	ShowLobbyMenuServer();
}

void MainMenu::onConnectButtonClicked()
{
	IP ip;
	string IPstring(IpInputBox.editbox->getText().c_str());
	string clientNameString(clientName.editbox->getText().c_str());

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
			Game::Instance()->SetClient(ip);
		}
	}
	HideConnectionMenu();
	ShowWaitingInfo();
}

void MainMenu::OndisconnectButtonClicked()
{
	//TODO:disconnect from lobby
	Game::Instance()->GetUser()->Disconnect();
	HideWaitingInfo();
	ShowConnectionMenu();
}

void MainMenu::ShowMainMenu()
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

void MainMenu::ShowConnectionMenu()
{
	connectToHostButton->enable();
	connectToHostButton->setVisible(true);

	connectMenuBack->enable();
	connectMenuBack->setVisible(true);

	IpInputBox.editbox->enable();
	IpInputBox.editbox->setVisible(true);
}

void MainMenu::HideConnectionMenu()
{
	connectToHostButton->setVisible(false);
	connectToHostButton->disable();

	IpInputBox.editbox->setVisible(false);
	IpInputBox.editbox->disable();

	connectMenuBack->disable();
	connectMenuBack->setVisible(false);
}

void MainMenu::ShowWaitingInfo()
{
	disconnectToHost->enable();
	disconnectToHost->setVisible(true);

	otherPlayersInfo->enable();
	otherPlayersInfo->setVisible(true);

	clientName.editbox->enable();
	clientName.editbox->setVisible(true);
}

void MainMenu::HideWaitingInfo()
{
	disconnectToHost->disable();
	disconnectToHost->setVisible(false);

	otherPlayersInfo->disable();
	otherPlayersInfo->setVisible(false);

	clientName.editbox->disable();
	clientName.editbox->setVisible(false);
}

void MainMenu::HideOptionMenu()
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

	enableBloomButton->disable();
	enableBloomButton->setVisible(false);

	disableBloomButton->disable();
	disableBloomButton->setVisible(false);
}

void MainMenu::HideMainMenu()
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

void MainMenu::ShowOptionMenu1()
{
	//Enable Option Menu
	OptionMenuBack->setVisible(true);
	OptionMenuBack->enable();
	mastervolumeSlider->setVisible(true);
	mastervolumeSlider->enable();
	GameSoundsSlider->setVisible(true);
	GameSoundsSlider->enable();
	MusicSlider->setVisible(true);
	MusicSlider->enable();

	mastervolumeSlider->activate();
	mastervolumeText->setVisible(true);
	GameSoundvolumeText->activate();
	GameSoundvolumeText->setVisible(true);
	MusicvolumeText->activate();
	MusicvolumeText->setVisible(true);

	CameraSensitivity->activate();
	CameraSensitivity->setVisible(true);
	CameraSensitivityText->activate();
	CameraSensitivityText->setVisible(true);

	VsyncText->activate();
	VsyncText->setVisible(true);
	enableVsync->activate();
	enableVsync->setVisible(true);
	disableVsync->activate();
	disableVsync->setVisible(true);
	background->setVisible(true);

	enableBloomButton->enable();
	enableBloomButton->setVisible(true);

	disableBloomButton->enable();
	disableBloomButton->setVisible(true);
}

void MainMenu::onMastervolumeChanged() { float temp = mastervolumeSlider->getCurrentValue(); AudioSystem::Instance()->SetMasterVolume(temp); }
void MainMenu::onGameSoundvolumeChanged() { float temp = GameSoundsSlider->getCurrentValue(); AudioSystem::Instance()->SetGameSoundsVolume(temp); }
void MainMenu::onMusicvolumeChanged() { float temp = MusicSlider->getCurrentValue(); AudioSystem::Instance()->SetGameSoundsVolume(temp); }

void MainMenu::Quit() {
	SceneManager::Instance()->SetExitButtonClicked(true);
}
