#pragma once
#include "../ncltech/GameObject.h"
#include "Player.h"


class PaintableGameObject {
public:
	PaintableGameObject(GameObject * g, float objWorth = 0, bool p = true) {
		go = g;
		objectWorth = objWorth;
		isPaintable = p;
	}
	virtual ~PaintableGameObject() {

	}

	GameObject * GetGameObject() {
		return go;
	}

	void SetPaintable(bool b) {
		isPaintable = b;
	}
	bool GetPaintable() const {
		return isPaintable;
	}

	virtual void CollisionOccured(Player * p) {
		if (isPaintable) {
			switch (p->GetColour()) {
				case Green: 
					go->Render()->GetChild()->SetBaseColor(Vector4(0.f, 1.0f, 0.0f, 1.0f));
					break;
				
				case Red: 
					go->Render()->GetChild()->SetBaseColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
					break;
				
				case Blue: 
					go->Render()->GetChild()->SetBaseColor(Vector4(0.f, 0.0f, 1.0f, 1.0f));
					break;
				
				case Pink: 
					go->Render()->GetChild()->SetBaseColor(Vector4(1.2f, 0.5f, 1.0f, 1.0f));
					break;
				
			}
			
		}
	}

protected:
	GameObject * go;
	//could also be used for other functionality like who owns an ammopool
	//can iterate through paintable objects and count the worths for each player
	//allows to easily see who has how many points
	float objectWorth;
	//isPaintable can be used for capture points
	//all players should have an ammo pool that can't be captured 
	//therefore can't be painted
	bool isPaintable;
};