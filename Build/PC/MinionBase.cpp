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
     .lo.            '0MMMMMMMMMMMMMMMWc            .cd,			MinionBase.cpp
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

#include "MinionBase.h"
#include "Game.h"
MinionBase::MinionBase()
{
	colour = START_COLOUR;
	RGBA = DEFAULT_COLOUR;
	life = 50;
	size = 0.3f;
}

MinionBase::MinionBase(Colour c, Vector4 RGBA, Vector3 position, const string name)
{
	size = 0.3f;
	RenderNode * rnode = new RenderNode();
	PhysicsNode * pnode = new PhysicsNode();

	RenderNode* dummy = new PlayerRenderNode(CommonMeshes::Sphere(), name, RGBA);
	dummy->SetTransform(Matrix4::Scale(Vector3(size, size, size)));
	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::Forward_Lighting]);
	rnode->AddChild(dummy);

	rnode->GetChild()->SetBaseColor(RGBA);
	rnode->SetTransform(Matrix4::Translation(position));

	pnode->SetBoundingRadius(size);
	rnode->SetBoundingRadius(size);

	pnode->SetPosition(position);
	lastPos = position;
	pnode->SetType(MINION);
	pnode->SetInverseMass(2.0f);

	CollisionShape* pColshape = new SphereCollisionShape(size);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(2.0f));
	pnode->SetFriction(0.0f);
	pnode->SetElasticity(1.0f);

	renderNode = rnode;

	RegisterPhysicsToRenderTransformCallback();

	SetPhysics(pnode);
	friendlyName = name;
	physicsNode->SetName(name);

	dead = false;

	minLife = 10;
	maxLife = 50;
	life = maxLife;

	colour = c;
	this->RGBA = RGBA;

	GraphicsPipeline::Instance()->AddPlayerRenderNode(dummy);
	((PlayerRenderNode*)Render()->GetChild())->SetIsInAir(false);

	isGrounded = false;

	Physics()->SetOnCollisionCallback(
		std::bind(&MinionBase::MinionCallbackFunction,
			this,							//Any non-placeholder param will be passed into the function each time it is called
			std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
			std::placeholders::_2
		)
	);
}

bool MinionBase::MinionCallbackFunction(PhysicsNode * self, PhysicsNode * collidingObject) {
	if (collidingObject->GetType() == PLAYER) {
		if (!dead) {
			if (((Avatar*)(collidingObject->GetParent()))->GetColour() != this->colour) {
				((Avatar*)collidingObject->GetParent())->ChangeLife(-(life / 5));
			}
			else ((Avatar*)(collidingObject->GetParent()))->ChangeLife(life / 5);
			dead = true;
		}
		return false;
	}
	else if (collidingObject->GetType() == MINION) {
		if (!dead && ((MinionBase*)(collidingObject->GetParent()))->IsAlive()) {
			if (((MinionBase*)(collidingObject->GetParent()))->GetColour() != this->colour) {
				float tempLife = life;
				ChangeLife(-((MinionBase*)collidingObject->GetParent())->GetLife());
				((MinionBase*)(collidingObject->GetParent()))->ChangeLife(-(tempLife));
				return false;
			}
		}
		return true;
	}
	if (collidingObject->GetType() == BIG_NODE || collidingObject->GetType() == DEFAULT_PHYSICS) {
		isGrounded = true;
		ChangeLife(-1);
		return true;
	}
	if (collidingObject->GetType() == PAINTABLE_OBJECT)
	{
		if (((CaptureArea*)(collidingObject->GetParent()))->GetColour() != this->colour)
		{
			dead = true;
		}
		isGrounded = true;
		
	}
	if (collidingObject->GetType() == PICKUP) {
		return false;
	}
	return true;
}


void MinionBase::ChangeLife(float l) {
	life += l;
	if (life < minLife) {
		dead = true;
	}
	if (life > maxLife) {
		life = maxLife;
	}
}

void MinionBase::SetLife(float l)
{
	life = l;
	if (life < minLife) {
		dead = true;
	}
	if (life > maxLife) {
		life = maxLife;
	}

	size = 0.3f * (life / 50);
	ChangeSize(size);
}

void MinionBase::ChangeSize(float newSize) {
	Render()->GetChild()->SetBoundingRadius(newSize);
	Render()->SetBoundingRadius(newSize);
	Physics()->SetBoundingRadius(newSize);
	((SphereCollisionShape*)Physics()->GetCollisionShape())->SetRadius(newSize);

	Render()->GetChild()->SetTransform(Matrix4::Scale(Vector3(newSize, newSize, newSize)));
}

void MinionBase::Update(float dt)
{
	
	if (dead)
	{
		this->SetToDestroy();
		Game::Instance()->KillMinion(this);
		((Map*)Game::Instance()->GetMap())->RemoveMinion(this);
		return;
	}

	size = 0.3f * (life / 50);
	ChangeSize(size);
}
