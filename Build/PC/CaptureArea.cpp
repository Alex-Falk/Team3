//					 ,.ood888888888888boo.,
//              .od888P^""            ""^Y888bo.
//          .od8P''   ..oood88888888booo.    ``Y8bo.
//       .odP'"  .ood8888888888888888888888boo.  "`Ybo.
//     .d8'   od8'd888888888f`8888't888888888b`8bo   `Yb.
//    d8'  od8^   8888888888[  `'  ]8888888888   ^8bo  `8b
//  .8P  d88'     8888888888P      Y8888888888     `88b  Y8.
// d8' .d8'       `Y88888888'      `88888888P'       `8b. `8b
//.8P .88P            """"            """"            Y88. Y8.
//88  888                 Nick Bedford                 888  88
//88  888             Capture Area Class               888  88
//88  888.        ..       16/02/2018       ..        .888  88
//`8b `88b,     d8888b.od8bo.      .od8bo.d8888b     ,d88' d8'
// Y8. `Y88.    8888888888888b    d8888888888888    .88P' .8P
//  `8b  Y88b.  `88888888888888  88888888888888'  .d88P  d8'
//    Y8.  ^Y88bod8888888888888..8888888888888bod88P^  .8P
//     `Y8.   ^Y888888888888888LS888888888888888P^   .8P'
//       `^Yb.,  `^^Y8888888888888888888888P^^'  ,.dP^'
//          `^Y8b..   ``^^^Y88888888P^^^'    ..d8P^'
//              `^Y888bo.,            ,.od888P^'
//                   "`^^Y888888888888P^^'"     

// Improved/Extended by Alex Falk

#include <ncltech\CuboidCollisionShape.h>
#include <string.h>
#include <nclgl\NCLDebug.h>
#include "Game.h"
#include "Avatar.h"
#include "Projectile.h"
#include "Minion.h"
#include "CaptureArea.h"
#include <nclgl\ChangeColorRenderNode.h>
#include "AudioSystem.h"
CaptureArea::CaptureArea()
{
	CaptureArea({ 0,0,0 },0.0f,"CAPTUREAREA");
}

CaptureArea::CaptureArea(Vector3 posit, float invmass, string unique_name, Vector3 halfdims, int scoreValue, Colour colour)
{
	Vector3 pos = posit;
	friendlyName = unique_name;
	Vector4 paintColour;

	this->colour = colour;

	RenderNode* rnode = new RenderNode();
	RenderNode* dummy = new ChangeColorRenderNode(CommonMeshes::Cube(), unique_name,paintColour);

	dummy->SetTransform(Matrix4::Scale(halfdims));

	dummy->SetMaterial(GraphicsPipeline::Instance()->GetAllMaterials()[MATERIALTYPE::ChangeColorObject]);

	rnode->AddChild(dummy);

	rnode->SetTransform(Matrix4::Translation(pos));
	rnode->SetBoundingRadius(halfdims.Length());

	PhysicsNode* pnode = NULL;

	pnode = new PhysicsNode();
	pnode->SetPosition(pos);
	pnode->SetInverseMass(invmass);
	pnode->SetName(unique_name);
	pnode->SetType(PAINTABLE_OBJECT);

	float x = halfdims.x*2.0f;
	float y = halfdims.y*2.0f;
	float z = halfdims.z*2.0f;
	float a;

	if (x >= y && x >= z) { a = x; }
	else if (y > x && y >= z) { a = y; }
	else { a = z; }

	pnode->SetBoundingRadius(a * sqrt(3.0f) / 2.0f);

	CollisionShape* pColshape = new CuboidCollisionShape(halfdims);
	pnode->SetCollisionShape(pColshape);
	pnode->SetInverseInertia(pColshape->BuildInverseInertia(invmass));
	this->renderNode = rnode;
	this->physicsNode = pnode;
RegisterPhysicsToRenderTransformCallback();
SetPhysics(pnode);

Physics()->SetOnCollisionCallback(
	std::bind(&CaptureArea::CaptureAreaCallbackFunction,
		this,							//Any non-placeholder param will be passed into the function each time it is called
		std::placeholders::_1,			//The placeholders correlate to the expected parameters being passed to the callback
		std::placeholders::_2
	)
);

	lifeReq = 10;
	this->scoreValue = scoreValue;
	SetColour(colour);
	UpdatePercentage();
	currentlyCapturing = RED;
	type = DEFAULT_CAPTURE_AREA;
	pnode->SetElasticity(0.05f);
}

void CaptureArea::SetColour(Colour c)
{
	colour = c;

	Vector4 paintColour;

	switch (colour)
	{
		case RED:			paintColour = RED_COLOUR;		playerScores[0] = lifeReq;	playerScores[1] = 0;		playerScores[2] = 0;		playerScores[3] = 0;		break;
		case GREEN:			paintColour = GREEN_COLOUR;		playerScores[0] = 0;		playerScores[1] = lifeReq;	playerScores[2] = 0;		playerScores[3] = 0;		break;
		case BLUE:			paintColour = BLUE_COLOUR;		playerScores[0] = 0;		playerScores[1] = 0;		playerScores[2] = lifeReq;	playerScores[3] = 0;		break;
		case PINK:			paintColour = PINK_COLOUR;		playerScores[0] = 0;		playerScores[1] = 0;		playerScores[2] = 0;		playerScores[3] = lifeReq;	break;
		case START_COLOUR:	paintColour = DEFAULT_COLOUR;	playerScores[0] = 0;		playerScores[1] = 0;		playerScores[2] = 0;		playerScores[3] = 0;		break;
	}

	static_cast<ChangeColorRenderNode*>(Render()->GetChild())->StartChangeColor();
	Render()->SetChildBaseColor(paintColour);
}

CaptureAreaType CaptureArea::GetType()
{
	return type;
}

void CaptureArea::SetType(CaptureAreaType newType)
{
	type = newType;
}

bool CaptureArea::CaptureAreaCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject)
{
	switch(collidingObject->GetType())
	{
		case PLAYER:		CheckPlayerCollision(collidingObject, Game::Instance()->GetUserID());		break;
		case PROJECTILE:
		case SPRAY:			CheckProjectileCollision(collidingObject, Game::Instance()->GetUserID());	break;
		case MINION:		CheckMinionCollision(collidingObject, Game::Instance()->GetUserID());		break;
	}
	
	//Return true to enable collision resolution
	return (this->GetType() == MINION_CAPTURE_AREA) ? false : true;

}
//----------------------------------------------------------------------------------------------//
//  AlexFalk

void CaptureArea::CheckPlayerCollision(PhysicsNode * p, int index) 
{
	Avatar * avatar = static_cast<Avatar*>(p->GetParent());

	if (this->colour != avatar->GetColour())
	{
		//calculate the amount of life it takes to capture
		float lifeToTake = lifeReq;
		for (int i = 0; i < 4; i++) {
			if (i == index) {
				lifeToTake -= playerScores[i];
			}
			else {
				lifeToTake += playerScores[i];
			}
		}
		//check if player actually has enough life to take the point
		if (avatar->GetLife() >= avatar->GetMinLife() + (lifeToTake)) {
			if (Game::Instance()->IsHost())
				this->SetColour(avatar->GetColour());
			AudioSystem::Instance()->PlayASound(CAPTURE_AREA_SOUND, false, this->Physics()->GetPosition());
			Game::Instance()->Capture(this->index, this->colour, this->scoreValue);
			avatar->ChangeLife(-lifeToTake);
		}
		UpdatePercentage();
	}
}
//----------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------//
// Nick
void CaptureArea::CheckMinionCollision(PhysicsNode * p, int index) 
{
	Minion* minion = static_cast<Minion*>(p->GetParent());
	
	if (this->GetColour() != minion->GetColour()) {
		
		float lifeToTake = ((Minion*)p->GetParent())->GetLife() / 10;
		minion->ChangeLife(-50);
		//take the amount of life of the minion from whoever is currently capturing the area
		for (int i = 0; i < 4; i++) {
			if (playerScores[i] > 0 && i != index) {
				float tempLifeToTake = lifeToTake;
				lifeToTake -= playerScores[i];
				playerScores[i] -= tempLifeToTake;
				if (playerScores[i] <= 0) {
					this->SetColour(START_COLOUR);
				}
			}
		}
		//if area has been set to neutral, add remaining life of minion to the player
		if (lifeToTake > 0) {
			for (int i = 0; i < 4; i++) {
				if (i == index) {
					playerScores[index] += lifeToTake;
				}
			}
			if (playerScores[index] >= lifeReq) {
				if (Game::Instance()->IsHost())
					this->SetColour(minion->GetColour());
				AudioSystem::Instance()->PlayASound(CAPTURE_AREA_SOUND, false, this->Physics()->GetPosition());
				Game::Instance()->Capture(this->index, this->colour,this->scoreValue);
			}
		}	
		UpdatePercentage();
	}
}

void CaptureArea::CheckProjectileCollision(PhysicsNode * p, int index) 
{
	Projectile* projectile = static_cast<Projectile*>(p->GetParent());

	if (this->GetColour() != projectile->GetColour()) 
	{
		float lifeToTake = projectile->GetProjectileWorth();
		for (int i = 0; i < 4; i++) {
			if (playerScores[i] > 0 && i != index) {
				float tempLifeToTake = lifeToTake;
				lifeToTake -= playerScores[i];
				playerScores[i] -= tempLifeToTake;
				if (playerScores[i] <= 0) {
					this->SetColour(START_COLOUR);
				}
			}
		}
		if (lifeToTake > 0) {
			for (int i = 0; i < 4; i++) {
				if (i == index) {
					playerScores[index] += lifeToTake;
					projectile->SetToDestroy();
				}
			}
			if (playerScores[index] >= lifeReq) {
				if (Game::Instance()->IsHost())
					this->SetColour(projectile->GetColour());
				AudioSystem::Instance()->PlayASound(CAPTURE_AREA_SOUND, false, this->Physics()->GetPosition());
				Game::Instance()->Capture(this->index, this->colour,this->scoreValue);
			}
		}
		UpdatePercentage();
	}

}
//----------------------------------------------------------------------------------------------//

void CaptureArea::UpdatePercentage() {
	bool updated = false;
	for (int i = 0; i < 4; i++) {
		if (Game::Instance()->GetPlayer(i)) {
			if (playerScores[i] > 0) {
				currentlyCapturing = Game::Instance()->GetPlayer(i)->GetColour();
				percentageCaptured = playerScores[i] / lifeReq;
				updated = true;
				break;
			}
		}
	}
	if (!updated) percentageCaptured = 0;
}

CaptureArea::~CaptureArea()
{
}
