//Nikos Fragkas
//Date 09/02/2018
//Impliment Weapons

#pragma once
#include <ncltech\GameObject.h>
#include <ncltech\SceneManager.h> 

float bulletPower = 100;
float rocketPower = 70;
float sprayPower = 3;
float launcherPower = 10;

namespace Weapons
{
	GameObject* BuildPistol(const Vector4& color, float size, Vector3 pos);

	GameObject* BuildRocket(const Vector4& color, float size, Vector3 pos);

	GameObject* BuildPaintSpray(const Vector4& colour, float size, Vector3 pos);



	vector<GameObject*> ShootPistol(Vector3 pos, float size, Vector4 colour);
	vector<GameObject*> ShootRocket(Vector3 pos, float size, Vector4 colour);
	vector<GameObject*> ShootPaintSpray(Vector3 pos, float size, Vector4 colour);
}