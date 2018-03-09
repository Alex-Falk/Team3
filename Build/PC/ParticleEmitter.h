#pragma once
#include <ncltech\GameObject.h>
#include "GamePlay.h"

class ParticleEmitter : public GameObject {
protected:
	Colour col;
	Vector3 pos;
	Vector3 dir;
	float rate;
	float spreadYaw;
	float spreadPitch;
	float particleLife;
	float particleMaxDist;
	float yaw = 0.0f;
	float t = 0.0f;
public:
	ParticleEmitter(Colour c, Vector3 pos, float rate, Vector3 direction = { 0,1,0 }, float spreadYaw = 45.0f, float spreadPitch = 45.0f, float particleLife = 5.0f, float particleMaxDist = 10.0f);
	~ParticleEmitter() {}

	virtual void Update(float dt);

	void SetDirection(Vector3 dir)	{ this->dir = dir; }
	void SetPosition(Vector3 pos)	{ this->pos = pos; }
};