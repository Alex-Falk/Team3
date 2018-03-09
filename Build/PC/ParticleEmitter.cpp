#include "ParticleEmitter.h"
#include "Particle.h"
#include <ncltech\SceneManager.h>

ParticleEmitter::ParticleEmitter(Colour c, Vector3 pos, float rate, Vector3 direction, float spreadYaw, float spreadPitch, float particleLife, float particleMaxDist)
{
	col = c;
	this->pos = pos;
	this->dir = direction;
	this->rate = rate;
	this->spreadYaw = spreadYaw;
	this->spreadPitch = spreadPitch;
	this->particleLife = particleLife;
	this->particleMaxDist = particleMaxDist;
}

void ParticleEmitter::Update(float dt)
{
	t += dt;

	if (t > 1.0f/rate)
	{
		int randPitch;
		int randYaw;

		yaw += 4;
		if (yaw >= 360.0f) { yaw = 0; }

		if (spreadPitch > 1.0f)
			randPitch = rand() % (int)spreadPitch;
		if (spreadYaw > 1.0f)
			randYaw = rand() % (int)spreadYaw;

		Vector3 direction = Matrix3::Rotation((float)randPitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation(yaw, Vector3(0.0f, 1.0f, 0.0f)) * dir * 10;

		Particle * particle = new Particle(col, pos, direction, 0.05f);
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(particle);

		t = 0;
	}


}