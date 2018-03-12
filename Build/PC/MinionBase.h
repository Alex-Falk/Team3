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
     .,;.            '0MMMMMMMMMMMMMMMWc             ';.			Alexander Falk & Michael Davis
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			MinionBase.h
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
// Base Class for minions, that stores necessary information but does not follow AI

#pragma once

#include "GamePlay.h"
#include "../ncltech/GameObject.h"

class ParticleEmitter;

class MinionBase : public GameObject {
protected:
	Colour colour;
	Vector4 RGBA;

	float life;
	float minLife;
	float maxLife;
	float size;
	float scale = 0.1f;

	Vector3 lastPos;

	bool isGrounded;
	bool dead;

	ParticleEmitter * emitter;

public:
	MinionBase();
	MinionBase(Colour c, Vector4 RGBA, Vector3 position, const string name = "");

	~MinionBase();

	void SetColour(Colour c) { colour = c; }
	Colour GetColour() { return colour; }

	float GetMinLife() { return minLife; }

	void SetRGBA(Vector4 r) { RGBA = r; }
	Vector4 GetRGBA() { return RGBA; }

	void SetDead(bool b) { dead = b; }

	void SetIsGrounded(bool b) { isGrounded = b; }
	bool GetIsGrounded() { return isGrounded; }

	void ChangeLife(float l);
	void SetLife(float l);
	float GetLife() { return life; }


	void ChangeSize(float newSize);

	virtual void Update(float dt);

	bool IsAlive() { return !dead; }

	virtual void OnDetachedFromScene();
};