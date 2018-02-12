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
		player = new Player(Vector3(0.0, 1.0, 0.0), DEFAULT, 0, 1.0f);

		this->AddGameObject(player->GetGameObject());

		GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
		GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(3000);

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
		CEGUI::PushButton* startButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("TaharezLook/Button",
				Vector4(0.40f, 0.25f, 0.2f, 0.1f),
				Vector4(),
				"startButton"
			));

		//Set text for testButton
		startButton->setText("NEW GAME ");
		startButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&MainMenu::onButtonClicked, this));

		CEGUI::PushButton* optionButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("WindowsLook/Button",
				Vector4(0.40f, 0.45f, 0.2f, 0.1f),
				Vector4(),
				"optionButton"
			));
		optionButton->setText("OPTION");

		CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(
			sceneGUI->createWidget("AlfiskoSkin/Button",
				Vector4(0.40f, 0.65f, 0.2f, 0.1f),
				Vector4(),
				"exitButton"
			));
		exitButton->setText(" EXIT ");
	}

	void onButtonClicked() {
		cout << "Zhang mingyuan idoit!" << endl;
		SceneManager::Instance()->JumpToScene();
	}
};