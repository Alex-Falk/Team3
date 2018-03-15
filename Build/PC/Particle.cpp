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

#include "Game.h"
#include "Particle.h"
#include <nclgl\Camera.h>

//TODO simplify this, possibly just remove this and do it in particle emitter
Particle::Particle(Colour col, Vector3 pos, Vector3 vel, Vector3 size, float lifeTime, float maxDist, string name)
{
	RenderNode * rnode = new RenderNode();

	RGB = EnumToVectorColour(col).ToVector3();

	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Cube(), "Particle", EnumToVectorColour(col));
	dummy->SetTransform(Matrix4::Scale(size));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ParticleRender]);
	dummy->SetHasShadow(false);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(EnumToVectorColour(col));
	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetHasShadow(false);

	position = pos;
	spawnPos = pos;
	velocity = vel*2.0f;
	this->lifeTime = lifeTime;
	this->maxDist = maxDist;

	renderNode = rnode;

	Game::Instance()->GetMap()->AddGameObject(this,1);
}

void Particle::SetPos(Vector3 pos)
{
	position = pos;
	renderNode->SetTransform(Matrix4::Translation(pos));
}

void Particle::SetScale(Vector3 scale)
{
	renderNode->GetChild()->SetTransform(Matrix4::Scale(scale));
}

void Particle::OnDetachedFromScene()
{

}

void Particle::SetColour(Colour c)
{
	this->colour = c;
	this->RGB = EnumToVectorColour(c).ToVector3();
	renderNode->SetBaseColor(EnumToVectorColour(c));
	renderNode->SetChildBaseColor(EnumToVectorColour(c));
}
