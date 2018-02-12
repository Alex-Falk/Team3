#include "WeaponPickup.h"
#include "SimpleGamePlay.h"

void SimpleGamePlay::OnInitializeScene() {

	if (!TextureManager::Instance()->LoadTexture(TEXTURETYPE::Checker_Board, TEXTUREDIR"checkerboard.tga", GL_REPEAT, GL_NEAREST))
		NCLERROR("Texture not loaded");

	//Create Ground (..everybody loves finding some common ground)
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

	pickup = new WeaponPickup(Vector3(0.0f, 3.0f, 0.0f), PAINT_ROCKET, 3.0f);
	
	this->AddGameObject(pickup->GetObj());

	GraphicsPipeline::Instance()->GetCamera()->SetCenter(player->GetGameObject()->Physics());
	GraphicsPipeline::Instance()->GetCamera()->SetMaxDistance(30);

	GraphicsPipeline::Instance()->InitPath(Vector2(40, 40));
	GraphicsPipeline::Instance()->AddPlayerRenderNode(player->GetGameObject()->Render());

	OnInitializeGUI();
	Scene::OnInitializeScene();
}

void SimpleGamePlay::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	m_AccumTime += dt;

	player->OnPlayerUpdate(dt);

	pickup->Update(dt);


	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();
}

void SimpleGamePlay::OnCleanupScene()
{
	DeleteAllGameObjects();
	TextureManager::Instance()->RemoteAllTexture();

	GraphicsPipeline::Instance()->RemoteAllPlayerRenderNode();
};

void SimpleGamePlay::OnInitializeGUI()
{
	//Call initi-function for gui
	sceneGUI = new GUI();
	sceneGUI->Init(CEGUIDIR);

	//Load Scheme - Which actually means UI style - notice that multiple Scheme could be load at once
	sceneGUI->LoadScheme("TaharezLook.scheme");
	sceneGUI->LoadScheme("AlfiskoSkin.scheme");

	//Set Font sytle
	sceneGUI->SetFont("DejaVuSans-10");

	//SetMouseCursor
	sceneGUI->SetMouseCursor("TaharezLook/MouseArrow");
	sceneGUI->ShowMouseCursor();

	//Create Push Button handle
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(
		sceneGUI->createWidget("AlfiskoSkin/Button",
			Vector4(0.5f, 0.5f, 0.1f, 0.05f),
			Vector4(),
			"TestButton"
		));

	//Create Titlebar
	CEGUI::Titlebar* Titlebar = static_cast<CEGUI::Titlebar*>(
		sceneGUI->createWidget("AlfiskoSkin/Titlebar",
			Vector4(0.0f, 0.0f, 0.1f, 0.05f),
			Vector4(),
			"Titlebar"
		));

	testButton->subscribeEvent(CEGUI::PushButton::EventMouseClick, CEGUI::Event::Subscriber(&SimpleGamePlay::onButtonClicked, this));

	//Set text for testButton
	testButton->setText("Hello, world!");
}

void SimpleGamePlay::onButtonClicked()
{
	cout << "Zhang mingyuan idoit!" << endl;
	SceneManager::Instance()->JumpToScene();
}