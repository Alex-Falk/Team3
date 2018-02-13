#pragma once
#include "../ncltech/GameObject.h"
#include "Player.h"
#include "GamePlay.h"

class PaintableGameObject  {
protected:
	GameObject * go;
	//who owns the current object. Class could be extended to a capture point with added functionality 
	Colour colour;
	//can iterate through paintable objects and count the worths for each player
	//allows to easily see who has how many points
	float objectWorth;
	//all players should have an ammo pool that can't be captured 
	//therefore can't be painted
	bool isPaintable;

public:
	PaintableGameObject(GameObject * g, float objWorth = 0, bool paintable = true, Colour colour = DEFAULT_COLOUR);

	GameObject * GetGameObject() {
		return go;
	}

	void SetColour(Colour colour) {

	}

	void SetObjectWorth(float objWorth) {
		objectWorth = objWorth;
	}

	float GetObjectWorth() const {
		return objectWorth;
	}

	void SetPaintable(bool b) {
		isPaintable = b;
	}

	bool GetPaintable() const {
		return isPaintable;
	}


	virtual void CollisionOccured(Player * p);

	//Used for collision with player ammo if/when implemented
	//virtual void CollisionOccured(Ammo * a);

	virtual ~PaintableGameObject() {

	}


};