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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			Particle.cpp
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

#include <ncltech\SceneManager.h>
#include "Particle.h"
#include <nclgl\Camera.h>

Particle::Particle(Colour col, Vector3 pos, Vector3 vel, float size, float lifeTime, float maxDist, string name)
{
	RenderNode * rnode = new RenderNode();

	RGB = EnumToVectorColour(col).ToVector3();

	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), "Particle", EnumToVectorColour(col));
	dummy->SetTransform(Matrix4::Scale(Vector3(size, size, size)));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(EnumToVectorColour(col));
	rnode->SetTransform(Matrix4::Translation(pos));

	position = pos;
	spawnPos = pos;
	velocity = vel*2.0f;
	this->lifeTime = lifeTime;
	this->maxDist = maxDist;

	renderNode = rnode;
}

void Particle::Update(float dt)
{
	velocity += Vector3(0, -9.81f, 0) * dt;
	position += velocity * dt;	
	lifeTime -= dt;

	float dist = (spawnPos - position).Length();

	//float alpha = max((1.0f - (dist / 10.0f)), 0.0f);

	//renderNode->SetChildColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	//renderNode->SetChildBaseColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	//renderNode->SetColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	//renderNode->SetBaseColor(Vector4(RGB.x, RGB.y, RGB.z, 0.1f));
	renderNode->SetTransform(Matrix4::Translation(position));

	if (dist > maxDist || lifeTime <= 0)
	{
		SceneManager::Instance()->GetCurrentScene()->RemoveGameObject(this);
	}	
}