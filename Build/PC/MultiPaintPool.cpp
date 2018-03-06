//Nikos Fragkas
//Date 01/03
//Paint Pool that will have your colour as soon as you capture the objects arround

//Nick Bedford
//Date: 02/03/2018
//Made some changes to how the paintpool worked 

#include "MultiPaintPool.h"
#include "Avatar.h"

MultiPaintPool::MultiPaintPool() : CaptureArea()
{
	this->SetName("PAINTPOOLCAPTUREAREA");
}

MultiPaintPool::MultiPaintPool(Vector3 pos, string unique_name, Vector3 halfdims, int scoreValue, Colour colour) : CaptureArea(pos, unique_name, halfdims, scoreValue, colour)
{
	this->SetName("PAINTPOOLCAPTUREAREA");
}

void MultiPaintPool::SetColour(Colour c)
{
	colour = c;

	Vector4 paintColour;

	switch (colour)
	{
	case RED:
		paintColour = RED_COLOUR;
		break;

	case GREEN:
		paintColour = GREEN_COLOUR;
		break;

	case BLUE:
		paintColour = BLUE_COLOUR;
		break;

	case PINK:
		paintColour = PINK_COLOUR;
		break;

	case START_COLOUR:
		paintColour = DEFAULT_COLOUR;
		break;

	default:
		paintColour = DEFAULT_COLOUR;
		break;
	}

	Render()->SetChildBaseColor(paintColour);

	for (auto itr = pools.begin(); itr != pools.end(); ++itr)
	{
		(*itr)->ChangeColour(c);
	}
}

void MultiPaintPool::AddPool(PaintPool* newPool)
{
	if (std::find(pools.begin(), pools.end(), newPool) == pools.end())
	{
		pools.push_back(newPool);
	}
}

MultiPaintPool::~MultiPaintPool()
{
}