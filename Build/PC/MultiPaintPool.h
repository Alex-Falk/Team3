//Nikos Fragkas
//Date 01/03
//Paint Pool that will have your colour as soon as you capture the objects arround

//Nick Bedford
//Date: 02/03/2018
//Made some changes to how the paintpool worked 

#pragma once
#include "CaptureArea.h"


#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

class Paintpool;

class MultiPaintPool : public CaptureArea
{
public:
	MultiPaintPool();
	MultiPaintPool(Vector3 pos, string unique_name, Vector3 halfdims = { 3.0f, 2.0f, 3.0f }, int scoreValue = 100, Colour colour = START_COLOUR);

	void SetColour(Colour c);
	void AddPool(PaintPool* newPool);

	~MultiPaintPool();

protected:

	std::vector<PaintPool*> pools;
};
