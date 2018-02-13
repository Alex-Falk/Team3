#pragma once

#include <nclgl\NCLDebug.h>
#include <ncltech\Scene.h>
#include <ncltech\SceneManager.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\CommonUtils.h>
#include "GamePlay.h"
#include "Pickup.h"
#include "Player.h"
#include <ncltech\TextureManager.h> 

// Scene that shows simple Sphere-Sphere, Sphere-Cube and Cube-Cube colissions

class MainMenu : public Scene
{
private:
	Player* player;
	bool ShowOptionMenu;

	CEGUI::PushButton* startButton;
	CEGUI::PushButton* optionButton;
	CEGUI::PushButton* OptionMenuBack;

public:

	//Exit Button
	CEGUI::PushButton* exitButton;

	MainMenu(const std::string& friendly_name)
		: Scene(friendly_name) {}

	~MainMenu()
	{
		TextureManager::Instance()->RemoteAllTexture();
		delete player;
	}

	void OnCleanupScene() {
		DeleteAllGameObjects();
		TextureManager::Instance()->RemoteAllTexture();
		GraphicsPipeline::Instance()->RemoteAllPlayerRenderNode();
	}

	virtual void OnInitializeScene() {
		GraphicsPipeline::Instance()->SetIsMainMenu(true);

		if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
			NCLERROR("Texture not loaded");

		GameObject* ground = CommonUtils::BuildCuboidObject(
			"Ground",
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(40.0f, 1.0f, 40.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(0.2f, 0.5f, 1.0f, 1.0f),
			MATERIALTYPE::Ground);

		this->AddGameObject(ground);

		player = new Player(Vector3(0.0, 1.0, 0.0), DEFAULT, 0, 1.0f);

		this->AddGameObject(player->GetGameObject());

		GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
		GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(10);

		OnInitializeGUI();
		Scene::OnInitializeScene();
	}

	virtual void OnUpdateScene(float dt) {
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

		//Create Push Button handle
		startButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("OgreTray/Button",
				Vector4(0.40f, 0.25f, 0.2f, 0.1f),
				Vector4(),
				"startButton"
			));

		//Set text for testButton
		startButton->setText("NEW GAME ");
		startButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onButtonClicked, this));

		optionButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("WindowsLook/Button",
				Vector4(0.40f, 0.45f, 0.2f, 0.1f),
				Vector4(),
				"optionButton"
			));
		optionButton->setText("OPTION");
		optionButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onOptionButtonClicked, this));


		exitButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("AlfiskoSkin/Button",
				Vector4(0.40f, 0.65f, 0.2f, 0.1f),
				Vector4(),
				"exitButton"
			));
		exitButton->setText(" EXIT ");

		OptionMenuBack = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("AlfiskoSkin/Button",
				Vector4(0.40f, 0.65f, 0.2f, 0.1f),
				Vector4(),
				"OptionMenuBack"
			));
		OptionMenuBack->setText("Back");
		OptionMenuBack->setVisible(false);
		OptionMenuBack->disable();
		OptionMenuBack->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::OnOptionMenuBackClicked, this));
	}

	void onButtonClicked() {
		SceneManager::Instance()->JumpToScene();
	}

	void onOptionButtonClicked()
	{
		startButton->setVisible(false);
		exitButton->setVisible(false);
		optionButton->setVisible(false);
		OptionMenuBack->setVisible(true);
	}

	void OnOptionMenuBackClicked() 
	{
		startButton->enable();
		startButton->setVisible(true);
		exitButton->enable();
		exitButton->setVisible(true);
		optionButton->enable();
		optionButton->setVisible(true);
		OptionMenuBack->disable();
		OptionMenuBack->setVisible(false);
	}
};