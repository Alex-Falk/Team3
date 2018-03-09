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
		SceneManager::Instance()->GetCurrentScene()->AddGameObject(particle,1);

		t = 0;
	}


}