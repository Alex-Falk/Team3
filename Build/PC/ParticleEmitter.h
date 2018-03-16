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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			ParticleEmitter.h
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

#pragma once
#include <ncltech\GameObject.h>
#include "GamePlay.h"

class Particle;

struct vec3 {
	float x;
	float y;
	float z;
	float buff;
};

class ParticleEmitter : public GameObject {
protected:

	//--------------------------------------------------------------------------------------------//
	// Compute shader specific variables
	//--------------------------------------------------------------------------------------------//

	GLuint posSBO;
	GLuint velSBO;
	GLuint startVelSBO;

	vec3 * positions;
	vec3 * velocities;
	vec3 * startingVels;

	Shader * shader;

	//--------------------------------------------------------------------------------------------//
	// other variables
	//--------------------------------------------------------------------------------------------//

	uint particleNum;
	vector<Particle*> particles;

	Colour c;
	Vector4 RGB;
	Vector3 pos;
	Vector3 scale;
	Vector3 direction;
	float rate;
	float lifeTime;
	bool isDeleting = true;

	float particleLifeTime;
	float particleMaxDist;

	float timer;

public:

	//--------------------------------------------------------------------------------------------//
	// Constructor, Deconstructor and General functions
	//--------------------------------------------------------------------------------------------//

	ParticleEmitter(
		uint numParticles, 
		Colour c, 
		Vector3 pos, 
		float rate = (1.0f / 60.0f), 
		float lifetime = 0.0f, 
		Vector3 scale = { 0.1f,0.1f,0.1f }, 
		Vector3 direction = { 0,1,0 }, 
		float spreadYaw = 10.0f, 
		float spreadPitch = 10.0f, 
		float particleLife = 5.0f, 
		float particleMaxDist = 10.0f);
	~ParticleEmitter();

	void SetPos(Vector3 pos) { this->pos = pos; }
	void SetScale(Vector3 scale) { this->scale = scale; }
	void SetDirection(Vector3 dir) { this->direction = dir; }
	void SetColour(Colour c);
	virtual void Update(float dt);

	virtual void OnDetachedFromScene();

};