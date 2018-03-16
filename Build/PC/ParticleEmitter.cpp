/*               
                          .,okkkd:.                          
                       .:x0KOdooxKKkl,.                      
                   .,oOKKxc'. .. .;oOX0d:.                   
                ...oKOo;. .;dO00xc.  'cxKO, ..               
            .,lk0l...  .:oxXMMMMMWOoc'  .. ,O0d:.            
         .:d0XOo;.     ;c..kMMMMMK;.;:.     'ckKKkc'.        
      'lkKKxc'  .,.        oWMMMMO.        ''  .:d0KOo;.     
     '0Wk;. .,loo:.        :NMMMMx.        ,loo:. .,oXNc     
     ,0X: .lKWMKl.         ,KMMMWo         .;kWWXx' .kNc     
     '0X; .OMMMMWXx;.      ,0MMMNl       'o0NMMMMN: .kWc     
     '0X; .k0d0NWMMW0o,..cxKWMMMMXkl,..ckNMMMWKxkK: .kWc     
     '0X; .kl  ':okKWMNKXWMMMMMMMMMMNKXWWXOdc,. ,O: .kWc     
     '0X;  ,.      .,oXMMMMMMMMMMMMMMMWk;.      .;. .kNc     
     .,;.            '0MMMMMMMMMMMMMMMWc             ';.			Alexander Falk
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			ParticleEmitter.cpp
     '0X: .:,     .,lkNMMMMMMMMMMMMMMMWKo:'.    .c' .OWl     
     '0X; .ko.':okXWMW0xkXWMMMMMMMMN0xkNMWN0xl;.:O: .OWc     
     '0X; .OX0NMMMWKx;.  .:xNMMMW0l,.  'lONMMMWKKX: .kWc     
     '0X: .OMMMMNkc.       '0MMMNc       .;dKWMMMN: .kWc     
     '0N: .;xKWKc.         ;XMMMWo          'kNXkl. .OWc     
     .xNKd:. .;loc.        cNMMMMk.       .;ol;. .,lONK;     
      .'lkKKkl,. .         dWMMWM0'        .  .:d0XOo;.      
          .:d0X0d,     ,l:;OMMMMMXl;lc.    .ckKKkc'          
             .,lxc.,c'. .:d0WMMMMXkl,. .;:.'dd:.             
                  .l0XOo;. .;oooc' .'cxKKx'                  
                    .,lkKKxc'.  .;oOK0d:.                    
                        .:d0K000KKkl,.                       
                           .,cll:.                            
*/

#include "ParticleEmitter.h"
#include "Particle.h"
#include <ncltech\SceneManager.h>

ParticleEmitter::ParticleEmitter(
	uint numParticles,				// Max number of particles
	Colour c,						// Colour of particles
	Vector3 pos,					// Position of Emitter
	float rate,						// Rate of spawning of particles - TODO: not used right now
	float lifetime,					// Lifetime of emitter
	Vector3 scale,					// Size of each particle
	Vector3 direction,				// Main direction of emission
	float spreadYaw,				// Spread in degrees in the yaw
	float spreadPitch,				// Spread in degrees in pitch
	float particleLife,				// Lifetime of particles
	float particleMaxDist)			// Furtherst distance particles can travel
{
	particleNum = numParticles;
	this->pos = pos;
	this->c = c;
	this->scale = scale;
	this->direction = direction;
	this->rate = rate;
	this->lifeTime = lifetime;

	this->particleLifeTime = particleLife;
	this->particleMaxDist = particleMaxDist;

	RGB = EnumToVectorColour(c);

	//Initialize array of positions,velocities and starting velocities to use for reading out data from the compute shader
	positions = new vec3[numParticles];
	velocities = new vec3[numParticles];
	startingVels = new vec3[numParticles];

	if (lifetime < 0.01f)
	{
		isDeleting = false;
	}

	// Positions storage buffer object, intially filled with position of emitter
	glGenBuffers(1, &posSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleNum * sizeof(vec3), NULL, GL_STATIC_DRAW);

	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

	vec3 *points = (vec3*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0, particleNum * sizeof(vec3), bufMask);

	for (uint i = 0; i < particleNum; ++i)
	{
		points[i].x = pos.x;
		points[i].y = pos.y;
		points[i].z = pos.z;
		points[i].buff = 1.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	// Velocity storage buffer object, intially set to zero
	glGenBuffers(1, &velSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleNum * sizeof(vec3), NULL, GL_STATIC_DRAW);
	
	vec3 *vels = (vec3*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), bufMask);

	float randPitch;
	float randYaw;
	float randScale;
	Vector3 velScale;
	for (uint i = 0; i < particleNum; ++i)
	{
		
		vels[i].x = 0;
		vels[i].y = 0;
		vels[i].z = 0;
		vels[i].buff = 1.0f;

		Particle * p = new Particle(c, pos, { 0, 10, 0}, scale);
		particles.push_back(p);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	// starting velocities storage buffer object, used to set velocities once a particle resets as a "new" particle
	glGenBuffers(1, &startVelSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, startVelSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleNum * sizeof(vec3), NULL, GL_STATIC_DRAW);

	vec3 *startvels = (vec3*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), bufMask);


	for (uint i = 0; i < particleNum; ++i)
	{
		randPitch = (float)((rand() % (int)spreadPitch*2) - spreadPitch);
		randYaw = (float)(rand() % (int)spreadYaw);
		randScale = (rand() % 5) / 100.0f;
		Vector3 dir = Matrix3::Rotation(randPitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation(randYaw, Vector3(0.0f, 1.0f, 0.0f)) * (direction + (direction*randScale));

		startvels[i].x = dir.x;
		startvels[i].y = dir.y;
		startvels[i].z = dir.z;
		startvels[i].buff = 1.0f;		
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	// Get a pointer to the compute shader from the graphics pipeline
	shader = GraphicsPipeline::Instance()->GetShader(SHADERTYPE::ParticleCompute);
	
}

ParticleEmitter::~ParticleEmitter()
{
	posSBO = NULL;
	velSBO = NULL;

	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		if (*itr)
		{
			SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(*itr);
		}

	}
	particles.clear();
}

void ParticleEmitter::OnDetachedFromScene()
{
	posSBO = NULL;
	velSBO = NULL;

	for (auto itr = particles.begin(); itr != particles.end(); ++itr)
	{
		if (*itr)
		{
			SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(*itr);
		}

	}
	particles.clear();
}

void ParticleEmitter::SetColour(Colour c)
{
	this->RGB = EnumToVectorColour(c);
}

void ParticleEmitter::Update(float dt)
{
	// If emitter has lifetime, reduce its lifetime
	if (isDeleting)
	{
		lifeTime -= dt;
	}

	// Remove it if it hits zero
	if (lifeTime <= 0.0f && isDeleting)
	{
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
	}
	else
	{
		// Run compute shader
		glUseProgram(shader->GetProgram());
		glUniform1fv(glGetUniformLocation(shader->GetProgram(), "dt"), 1, &dt);
		glUniform1fv(glGetUniformLocation(shader->GetProgram(), "maxDist"), 1, &particleMaxDist);
		glUniform3fv(glGetUniformLocation(shader->GetProgram(), "startPos"), 1, (float*)&pos);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, startVelSBO);

		glDispatchCompute(particleNum, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glUseProgram(0);

		// Get Outcome of compute shader
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSBO);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), positions);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSBO);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), velocities);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		// Update ingame particle positions and colours
		for (uint i = 0; i < particles.size(); ++i)
		{
			if (particles[i])
			{
				particles[i]->SetPos({ positions[i].x,positions[i].y,positions[i].z });

				particles[i]->Render()->SetChildColor(Vector4(RGB.x, RGB.y, RGB.z, positions[i].buff));
				particles[i]->Render()->SetChildBaseColor(Vector4(RGB.x, RGB.y, RGB.z, positions[i].buff));

				// used to check if the particle has reset
				if (velocities[i].buff >= 0.9f)
				{
					particles[i]->SetScale(scale);
					particles[i]->SetPos({ 0,0,0 });
					// If the rate is zero, particle only spawn once -> delete particle after it reached the reset point
					if (rate < 0.0001f)
					{
						SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(particles[i]);
						particles[i] = nullptr;
					}
				}
			}
		}
	}	
}