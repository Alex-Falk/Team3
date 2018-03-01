//Nikos Fragkas
//Date 01/03
//Paint Pool that will have your colour as soon as you capture the objects arround

#pragma once
#include "PickUp.h"
#include "CaptureArea.h"
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

class MultiPaintPool : public Pickup
{
public:
	MultiPaintPool();
	MultiPaintPool(Vector3 pos, string unique_name, Colour colour = START_COLOUR, float respawnTime = 30.0);
	~MultiPaintPool();
	void UpdateMultiColourToPool();
	CaptureArea* GetPoolCaptArea(int id) { return poolCaptArea[id]; }
	void AddCaptAreaToPool(CaptureArea* CaptureArea) { poolCaptArea.push_back(CaptureArea); }

	bool PickupCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);

	void ChangeColour(Colour newColour);
	void ChangeSize(Vector3 newSize);


protected:
	vector<CaptureArea*> poolCaptArea;
	Colour colour;
};
