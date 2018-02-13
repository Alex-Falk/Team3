//Nikos Fragkas
//Date 09/02/2018
//Impliment Weapons

#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\SceneManager.h> 
#include "Projectile.h"

namespace Weapons
{
	GameObject* BuildPistol(const Vector4& color, float size, Vector3 pos);

	GameObject* BuildRocket(const Vector4& color, float size, Vector3 pos);

	GameObject* BuildPaintSpray(const Vector4& colour, float size, Vector3 pos);



	//vector<GameObject*> ShootPistol(Vector3 pos, float size, Vector4 colour);
	vector<GameObject*> ShootRocket(Vector3 pos, float size, Vector4 colour);
	vector<GameObject*> ShootPaintSpray(Vector3 pos, float size, Vector4 colour);

	GameObject * ShootPistol(Vector3 pos, float size, Vector4 colour);

	Projectile * ShootPistolProj(Vector3 pos, float size, Vector4 RGBA, Colour col);
}