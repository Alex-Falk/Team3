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
	uint numParticles,
	Colour c,
	Vector3 pos, 
	float rate,
	float lifetime,
	Vector3 scale, 
	Vector3 direction, 
	float spreadYaw, 
	float spreadPitch, 
	float particleLife, 
	float particleMaxDist)
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

	if (lifetime < 0.01f)
	{
		isDeleting = false;
	}


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

	glGenBuffers(1, &velSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleNum * sizeof(vec3), NULL, GL_STATIC_DRAW);

	vec3 *vels = (vec3*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), bufMask);

	if (rate < 0.0001f)
	{
		int randPitch;
		int randYaw;

		for (uint i = 0; i < particleNum; ++i)
		{


			randPitch = rand() % 90;
			randYaw = rand() % 360;
			Vector3 dir = Matrix3::Rotation((float)randPitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation((float)randYaw, Vector3(0.0f, 1.0f, 0.0f)) * direction;
			
			vels[i].x = dir.x;
			vels[i].y = dir.y;
			vels[i].z = dir.z;
			vels[i].buff = 1.0f;
			
			Particle * p = new Particle(c, pos, { vels[i].x,vels[i].y,vels[i].z }, scale);

			particles.push_back(p);
		}
	}
	else
	{
		for (uint i = 0; i < particleNum; ++i)
		{
			vels[i].x = 0.0f;
			vels[i].y = 0.0f;
			vels[i].z = 0.0f;
			vels[i].buff = 1.0f;
		}
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	positions = new vec3[numParticles];
	velocities = new vec3[numParticles];

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

void ParticleEmitter::Update(float dt)
{
	int randPitch;
	int randYaw;


	if (rate >= 0.0001f)
	{
		timer += dt;
		if (timer > rate)
		{
			if ((uint)particles.size() < particleNum - 1)
			{
				randPitch = rand() % 90;
				randYaw = rand() % 360;
				Vector3 dir = Matrix3::Rotation((float)randPitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation((float)randYaw, Vector3(0.0f, 1.0f, 0.0f)) * direction;
				particles.push_back(new Particle(c, pos, dir, scale));
			}
			timer = 0.0f;
		}
	}

	if (isDeleting)
	{
		lifeTime -= dt;
	}

	if (lifeTime <= 0.0f && isDeleting)
	{
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
	}
	else
	{
		glUseProgram(shader->GetProgram());
		glUniform1fv(glGetUniformLocation(shader->GetProgram(), "dt"), 1, &dt);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velSBO);

		glDispatchCompute(particleNum / 2, 2, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glUseProgram(0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSBO);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), positions);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSBO);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), velocities);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		float dist = particleMaxDist + 1.0f;
		float alpha;

		for (uint i = 0; i < particles.size(); ++i)
		{
			if (particles[i])
			{
				particles[i]->SetPos({ positions[i].x,positions[i].y,positions[i].z });

				if (particleMaxDist > 0.1f)
				{
					dist = (particles[i]->GetPos() - this->pos).Length();
					alpha = max((1.0f - (dist / particleMaxDist)), 0.0f);

					particles[i]->Render()->SetChildColor(Vector4(RGB.x, RGB.y, RGB.z, alpha));
					particles[i]->Render()->SetChildBaseColor(Vector4(RGB.x, RGB.y, RGB.z, alpha));
				}				

				if (particles[i]->GetPos().y <= 0.0f || dist >= particleMaxDist)
				{
					positions[i].x = pos.x;
					positions[i].y = pos.y;
					positions[i].z = pos.z;
					positions[i].buff = 1;

					randPitch = rand() % 90;
					randYaw = rand() % 360;
					Vector3 dir = Matrix3::Rotation((float)randPitch, Vector3(1.0f, 0.0f, 0.0f)) * Matrix3::Rotation((float)randYaw, Vector3(0.0f, 1.0f, 0.0f)) * direction * 1;

					velocities[i].x = dir.x;
					velocities[i].y = dir.y;
					velocities[i].z = dir.z;
					velocities[i].buff = 1;

					particles[i]->SetScale(scale);

					if (rate < 0.0001f)
					{
						SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(particles[i]);
						particles[i] = nullptr;
					}
				}
			}
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), positions);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particleNum * sizeof(vec3), velocities);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	
}