//Nikos Fragkas
//Date 01/03
//Paint Pool that will have your colour as soon as you capture the objects arround

//Nick Bedford
//Date: 02/03/2018
//Made some changes to how the paintpool worked 

#include "Paintpool.h"
#include "MultiPaintPool.h"
#include "Game.h"

MultiPaintPool::MultiPaintPool() : CaptureArea()
{
}

MultiPaintPool::MultiPaintPool(Vector3 pos, string unique_name, Vector3 halfdims, int scoreValue, Colour colour) : CaptureArea(pos, unique_name, halfdims, scoreValue, colour)
{
}

void MultiPaintPool::SetColour(Colour c)
{
	colour = c;

	Render()->SetChildBaseColor(EnumToVectorColour(colour));

	for (auto itr = pools.begin(); itr != pools.end(); ++itr)
	{
		(*itr)->ChangeColour(c);
		Game::Instance()->Capture((*itr)->GetIdx(), c);
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