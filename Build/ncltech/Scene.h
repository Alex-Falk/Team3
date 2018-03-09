/******************************************************************************
Class: Scene
Implements: 
Author: 
	Pieran Marris <p.marris@newcastle.ac.uk>
Description: 

	The Scene class is an extrapolation of the Scene Management tutorial 
	from Graphics for Games module. It contains a flat list of GameObject's, which in
	turn may have scene (old SceneNode's) or PhysicsNodes. The scene's responsibility
	is to automatically initiate and destroy those game objects on demand.

	With the addition of the SceneManager class, multiple scenes can cohexist within the same
	program meaning the same Scene could be initialied/cleaned up multiple times. The standard procedure
	for a Scene lifespan follows:-
		1. Constructor()		 [Program Start]
		2. OnInitializeScene()	 [Scene Focus]
		3. OnCleanupScene()		 [Scene Lose Focus]
		4. Deconsructor()		 [Program End]

	Once an object is added to the scene via AddGameObject(), the object is managed by the Scene. 
	This means that it will automatically call delete on any objects you have added when the scene 
	becomes innactive (lose focus). To override this you will need to override the OnCleanupScene method
	and handle cleanup of Objects yourself.

	For example usage see Tuts_Physics project.

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GameObject.h"
#include "MultiGameObject.h"
#include "PhysicsEngine.h"
#include <nclgl\NCLDebug.h>
#include <nclgl\TSingleton.h>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <PC\UserInterface.h>

//Callback function called whenever the scene is updated
// - Should be used to register Update(dt) functions for AI/Game Logic
//Params:
//	float dt - Elapsed time (in seconds) since last frame
typedef std::function<void(float dt)> SceneUpdateCallback;


//Storage map of all callback functions with a void* identifier
//	- In order to delete callback's when they are no longer needed
//    a void* identifier is used to identify each callback.
//    Usually this will be a "this" pointer refering to the class that setup the callback
typedef std::unordered_map<void*, SceneUpdateCallback> SceneUpdateMap;

class Scene
{
public:

	Scene(const std::string& friendly_name)	//Called once at program start - all scene initialization should be done in 'OnInitializeScene'
		: m_SceneName(friendly_name)
	{}; 

	~Scene()
	{
		DeleteAllGameObjects();
		m_UpdateCallbacks.clear();
	}


	// Called when scene is being activated, and will begin being rendered/updated. 
	//	 - Initialize objects/physics here
	virtual void OnInitializeScene()	{ 
		PhysicsEngine::Instance()->ResetWorldPartition();
	}

	// Called when scene is being swapped and will no longer be rendered/updated 
	//	 - Override to remove custom objects/physics here as needed
	//	   Note: Default action here automatically delete all game objects and
	//           remove all update callback's.
	virtual void OnCleanupScene()		{ DeleteAllGameObjects(); };	

	// Update Scene Logic
	//   - Called once per frame and should contain time-sensitive update logic
	//	   Note: This is time relative to seconds not milliseconds! (e.g. msec / 1000)
	//	   michael davis: projectiles delete after 10 seconds
	virtual void onConnectToScene() {}
	virtual void OnUpdateScene(float dt) {
		//vector<GameObject*> tempVec;
		//for (int i = 0; i < m_vpObjects.size(); i++) {
		//	m_vpObjects[i]->SetTimeInScene(m_vpObjects[i]->GetTimeInScene() + dt);
		//	if (m_vpObjects[i]->Physics()->GetType() == PROJECTILE || m_vpObjects[i]->Physics()->GetType() == SPRAY) {
		//		if (m_vpObjects[i]->GetTimeInScene() > 10.0f) {
		//			m_vpObjects[i]->SetToDestroy();
		//			//tempVec.push_back(m_vpObjects[i]);
		//		}
		//	}
		//}
		//for (int i = 0; i < tempVec.size(); i++) {
		//	RemoveGameObject(tempVec[i]);
		//}
	}

	//Timers For The Scenes
	void PrintPerformanceTimers(const Vector4& color)
	{
		perfPlayer.PrintOutputToStatusEntry(color,		"              Player Update  :");
		perfAI.PrintOutputToStatusEntry(color,			"              AI Update      :");
		perfMapObjects.PrintOutputToStatusEntry(color,	"              Objects Update :");
	}

	// Should be the action fired by the main game loop when updating a scene
	//   - This will call the above OnUpdateScene function along with any and all
	//     associated GameObject update callbacks.
	void FireOnSceneUpdate(float dt)
	{
		OnUpdateScene(dt); //Should this just be an optional callback too?

		for (auto&& callbackItr : m_UpdateCallbacks)
		{
			if (callbackItr.second) callbackItr.second(dt);
		}
	}

	// Add GameObject to the scene list
	//    - All added game objects are managed by the scene itself, firing
	//		OnRender and OnUpdate functions automatically
	void AddGameObject(GameObject* game_object, bool dynamic = false)
	{

		if (game_object)
		{
			if (game_object->scene) game_object->scene->RemoveGameObject(game_object);				

			if (dynamic)
			{
				game_object->SetIdx(mapDynamicObjects.size());
				game_object->SetDynamic(true);
				mapDynamicObjects.push_back(game_object);
			}
			else
			{
				game_object->SetIdx(mapConstantObjects.size());
				mapConstantObjects.push_back(game_object);
			}

			game_object->scene = this;
			game_object->OnAttachedToScene();

			if (game_object->renderNode) GraphicsPipeline::Instance()->AddRenderNode(game_object->renderNode);
			if (game_object->physicsNode) PhysicsEngine::Instance()->AddPhysicsObject(game_object->physicsNode);
		}
	}

	vector<GameObject*> GetConstantGameObjects()
	{
		return mapConstantObjects;
	}

	GameObject* GetGameObject(uint idx)
	{
		if (idx < mapConstantObjects.size())
		{
			if (mapConstantObjects[idx]->index == idx)
			{
				return mapConstantObjects[idx];
			}
			else
			{
				for (GameObject * go : mapConstantObjects)
				{
					if (go->GetIdx() == idx)
					{
						return go;
					}
				}
			}
		}
		return nullptr;
	}

	void AddMultiGameObject(MultiGameObject* game_object)
	{
		if (game_object)
		{
			//if (game_object->scene) game_object->scene->RemoveGameObject(game_object);

			mapConstantObjects.push_back(game_object);
			game_object->scene = this;
			game_object->OnAttachedToScene();

			if (game_object->renderNode) GraphicsPipeline::Instance()->AddRenderNode(game_object->renderNode);
			if (game_object->physicsNodes[0]) {
				for (std::vector<PhysicsNode*>::iterator it = game_object->physicsNodes.begin()
					; it != game_object->physicsNodes.end(); ++it)
				{
					PhysicsEngine::Instance()->AddPhysicsObject(*it);
				}
			}
		}
	}

	// Remove GameObject from the scene list
	//		- This will just remove it from the list of game objects,
	//		  it will not call any delete functions.
	void RemoveGameObject(GameObject* game_object)
	{
		if (!game_object->IsDynamic())
		{
			if (game_object && game_object->scene == this)
			{
				if (game_object->renderNode) GraphicsPipeline::Instance()->RemoveRenderNode(game_object->renderNode);
				if (game_object->physicsNode) PhysicsEngine::Instance()->RemovePhysicsObject(game_object->physicsNode);

				mapConstantObjects.erase(std::remove(mapConstantObjects.begin(), mapConstantObjects.end(), game_object), mapConstantObjects.end());
				game_object->OnDetachedFromScene();
				game_object->scene = NULL;
				SAFE_DELETE(game_object);
			}
		}
		else
		{
			if (game_object && game_object->scene == this)
			{
				if (game_object->renderNode) GraphicsPipeline::Instance()->RemoveRenderNode(game_object->renderNode);
				if (game_object->physicsNode) PhysicsEngine::Instance()->RemovePhysicsObject(game_object->physicsNode);

				mapDynamicObjects.erase(std::remove(mapDynamicObjects.begin(), mapDynamicObjects.end(), game_object), mapDynamicObjects.end());
				game_object->OnDetachedFromScene();
				game_object->scene = NULL;
				SAFE_DELETE(game_object);
			}
		}

	}


	// Simple Iterative Search
	//   - Searches all contained GameObject's and returns the first one with the name specified
	//     or NULL if none can be found.
	GameObject* FindGameObject(const std::string& name)
	{
		for (GameObject* obj : mapConstantObjects)
		{
			if (obj->GetName() == name)
				return obj;
		}
		return NULL;
	}

	GameObject* FindDynamicGameObject(const std::string& name)
	{
		for (GameObject* obj : mapDynamicObjects)
		{
			if (obj->GetName() == name)
				return obj;
		}
	}

	// The friendly name associated with this scene instance
	const std::string& GetSceneName() { return m_SceneName; }

	
	//Add update callback
	//  Any game object (or otherwise) can start listening for game update's, which will
	//  be fired once per frame with a 'float dt' parameter that will describe the seconds since the 
	//  last time the update callback was fired. Seeing as we may have lots of objects, and very few
	//  of them need game logic/AI the callback method allows us to only update those who need updating.
	//
	//  Note: The identifier here is used solely to allow Unregistering the callback later, but regardless
	//        of whether the callback is every removed, needs to be a unique ID. For normal use cases, passing
	//        in the "this" parameter of the class will suffice, and for cases where you need multiple update
	//        callbacks for the same class just use offsets.
	void RegisterOnUpdateCallback(void* identifier, SceneUpdateCallback callback)
	{
		m_UpdateCallbacks[identifier] = callback;
	}


	void UnregisterOnUpdateCallback(void* identifier)
	{
		for (SceneUpdateMap::iterator it = m_UpdateCallbacks.begin(); it != m_UpdateCallbacks.end();)
		{
			if ((it->first) == identifier)
				it = m_UpdateCallbacks.erase(it);
			else
				it++;
		}
	}

	//toggle whether the camera is following an object
	inline void ToggleCamera() {
		GraphicsPipeline::Instance()->GetCamera()->ToggleFree();
	}

	virtual void OnInitializeGUI(){}

protected:
	// Delete all contained Objects
	//    - This is the default action upon firing OnCleanupScene()
	void DeleteAllGameObjects()
	{
		m_UpdateCallbacks.clear();

		for (auto obj : mapConstantObjects)
			delete obj;
		
		mapConstantObjects.clear();

		for (auto obj : mapDynamicObjects)
			delete obj;

		mapDynamicObjects.clear();
	}


protected:
	std::string					m_SceneName;
	std::vector<GameObject*>	mapConstantObjects;
	std::vector<GameObject*>	mapDynamicObjects;
	SceneUpdateMap				m_UpdateCallbacks;

	// Timing Variables
	PerfTimer perfPlayer;
	PerfTimer perfAI;
	PerfTimer perfMapObjects;

};