#include "MainMenu.h"

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
	GraphicsPipeline::Instance()->SetIsMainMenu(true);
	GUIsystem::Instance()->SetDrawScoreBar(false);

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

void MainMenu::OnUpdateScene(float dt)
{
	float yaw = GraphicsPipeline::Instance()->GetCamera()->GetYaw();
	yaw += 0.1f;
	GraphicsPipeline::Instance()->GetCamera()->SetYaw(yaw);
	Scene::OnUpdateScene(dt);
}

void MainMenu::OnInitializeGUI()
{
	SetUpMainMenu();
	SetUpOptionMenu();
	SetUpLobby();
	SetUpconnectionMenu();
	HideLobby();
	HideConnectionMenu();
	HideOptionMenu();
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
			Vector4(0.40f, 0.75f, 0.2f, 0.1f),
			Vector4(),
			"startButton"
		));
	startButton->setText("START GAME");
	startButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onStartGameClicked, this));

	ipText = static_cast<CEGUI::DefaultWindow*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
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
	Map1Text->setText("MAP1: xxxxxxxx");

	Map2Text = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.65f, 0.30f, 0.20f, 0.1f),
			Vector4(),
			"Map2Text"
		));
	Map2Text->setText("MAP2: xxxxxxxx");

	Map3Text = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.65f, 0.50f, 0.20f, 0.1f),
			Vector4(),
			"Map3Text"
		));
	Map3Text->setText("MAP3: xxxxxxxx");

	Map4Text = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.65f, 0.70f, 0.20f, 0.1f),
			Vector4(),
			"Map4Text"
		));
	Map4Text->setText("MAP4: xxxxxxxx");
	
	inputBox temp;
	temp.editbox = static_cast<CEGUI::Editbox*>(
		GUIsystem::Instance()->createWidget("OgreTray/Editbox",
			Vector4(0.40f, 0.60f, 0.2f, 0.05f),
			Vector4(),
			"userName"
		));
	temp.type = "UserName";
	temp.editbox->setText("Please type your name here");
	temp.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnUserNameClicked, this));
	GUIsystem::Instance()->editboxes.push_back(temp);
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

}

void MainMenu::SetUpconnectionMenu()
{
	IpInputBox.editbox = static_cast<CEGUI::Editbox*>(
		GUIsystem::Instance()->createWidget("OgreTray/Editbox",
			Vector4(0.40f, 0.65f, 0.2f, 0.1f),
			Vector4(),
			"textBox"
		));
	IpInputBox.editbox->setText("127.0.0.1:1234");
	IpInputBox.editbox->subscribeEvent(CEGUI::Editbox::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnUserNameClicked, this));
	IpInputBox.type = "test";
	connectIP.type = IpInputBox.type;
	GUIsystem::Instance()->editboxes.push_back(IpInputBox);
	GUIsystem::Instance()->userTyping.push_back(connectIP);

	connectToHostButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.75f, 0.2f, 0.1f),
			Vector4(),
			"connectToHostButton"
		));
	connectToHostButton->setText("CONNECT TO GAME");
	connectToHostButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onConnectButtonClicked, this));
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
		GUIsystem::Instance()->createWidget("OgreTray/Slider",
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
		GUIsystem::Instance()->createWidget("OgreTray/Slider",
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
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.145f, 0.12f, 0.04f),
			Vector4(),
			"mastervolumeText"
		));
	mastervolumeText->setText("Master volume");

	GameSoundvolumeText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.245f, 0.12f, 0.04f),
			Vector4(),
			"GameSoundvolumeText"
		));
	GameSoundvolumeText->setText("Game Sound volume");

	MusicvolumeText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.345f, 0.12f, 0.04f),
			Vector4(),
			"MusicvolumeText"
		));
	MusicvolumeText->setText("Music volume");

	//Camera Sensitivity Control
	//TODO: link with the actual camera sensitivity
	CameraSensitivity = static_cast<CEGUI::Slider*>(
		GUIsystem::Instance()->createWidget("OgreTray/Slider",
			Vector4(0.40f, 0.45f, 0.30f, 0.03f),
			Vector4(),
			"CameraSensitivity"
		));
	CameraSensitivity->setMaxValue(100.0f);
	CameraSensitivity->setCurrentValue(100.0f);

	CameraSensitivityText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.445f, 0.12f, 0.04f),
			Vector4(),
			"CameraSensitivityText"
		));
	CameraSensitivityText->setText("Camera Sensitivity");
	CameraSensitivityText->deactivate();
	CameraSensitivityText->setVisible(false);

	background = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.40f, 0.535f, 0.24f, 0.07f),
			Vector4(),
			"VsyncBackground"
		));
	background->setText("Enable                   Disable");
	background->disable();
	background->setVisible(false);

	VsyncText = static_cast<CEGUI::Titlebar*>(
		GUIsystem::Instance()->createWidget("OgreTray/Title",
			Vector4(0.28f, 0.545f, 0.12f, 0.04f),
			Vector4(),
			"VsyncText"
		));
	VsyncText->setText("Vsync Setting");
	VsyncText->disable();

	enableVsync = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.45f, 0.545f, 0.08f, 0.05f),
			Vector4(),
			"EnableVsync"
		));
	enableVsync->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnEnableVsyncClicked, this));
	enableVsync->setSelected(true);

	disableVsync = static_cast<CEGUI::RadioButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/RadioButton",
			Vector4(0.55f, 0.545f, 0.08f, 0.05f),
			Vector4(),
			"DisableVsync"
		));
	disableVsync->subscribeEvent(CEGUI::Slider::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnDisableVsyncClicked, this));

	DebugButton = static_cast<CEGUI::PushButton*>(
		GUIsystem::Instance()->createWidget("OgreTray/Button",
			Vector4(0.40f, 0.65f, 0.2f, 0.1f),
			Vector4(),
			"debugButton"
		));
	DebugButton->setText("Debug Rendering");
	DebugButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnDebugRenderClicked, this));
}

void MainMenu::SetUpCreateGameMenu()
{

}

void MainMenu::onCreateGameClicked()
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

void MainMenu::onConnectButtonClicked()
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

	IpInputBox.editbox->enable();
	IpInputBox.editbox->setVisible(true);
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

	DebugButton->disable();
	DebugButton->setVisible(false);
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

void MainMenu::HideConnectionMenu()
{
	connectToHostButton->setVisible(false);
	connectToHostButton->disable();

	IpInputBox.editbox->setVisible(false);
	IpInputBox.editbox->disable();
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

	DebugButton->enable();
	DebugButton->setVisible(true);
}

