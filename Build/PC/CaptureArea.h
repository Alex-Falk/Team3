//					 ,.ood888888888888boo.,
//              .od888P^""            ""^Y888bo.
//          .od8P''   ..oood88888888booo.    ``Y8bo.
//       .odP'"  .ood8888888888888888888888boo.  "`Ybo.
//     .d8'   od8'd888888888f`8888't888888888b`8bo   `Yb.
//    d8'  od8^   8888888888[  `'  ]8888888888   ^8bo  `8b
//  .8P  d88'     8888888888P      Y8888888888     `88b  Y8.
// d8' .d8'       `Y88888888'      `88888888P'       `8b. `8b
//.8P .88P            """"            """"            Y88. Y8.
//88  888                 Nick Bedford                 888  88
//88  888             Capture Area Class               888  88
//88  888.        ..       16/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"  
#pragma once

#include "GamePlay.h"
#include <ncltech\GameObject.h>

class CaptureArea : public GameObject
{
public:
	CaptureArea();
	CaptureArea(Vector3 pos, float invmass, string unique_name, Vector3 halfdims = { 3.0f, 0.5f, 3.0f }, int scoreValue = 100, Colour colour = START_COLOUR);

	virtual bool CaptureAreaCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	
	Colour GetColour() { return colour; }
	int GetScoreValue() { return scoreValue; }

	virtual void SetColour(Colour c);
	void SetScoreValue(int i) { scoreValue = i; }

	void SetLifeReq(float l) { lifeReq = l; }
	float GetLifeReq() { return lifeReq; }
	
	CaptureAreaType GetType();
	void SetType(CaptureAreaType newType);


	virtual void Update(float dt) {}

	void CheckPlayerCollision(PhysicsNode * p, int index);
	void CheckProjectileCollision(PhysicsNode * p, int index);
	void CheckMinionCollision(PhysicsNode * p, int index);
	
	//useful for GUI capture bar
	float GetPercentageCaptured() { return percentageCaptured; } //value between 0 and 1, fill bar this amount
	Colour GetCurrentlyCapturing() { return currentlyCapturing; } //the colour of whoever is capturing, colour the bar this colour

	void UpdatePercentage();

	~CaptureArea();
	
protected:

	Colour currentlyCapturing;
	float percentageCaptured;

	float playerScores[4];
	float lifeReq;
	int scoreValue;
	Colour colour;
	int life;
	CaptureAreaType type;
};

