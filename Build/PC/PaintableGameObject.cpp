#include "PaintableGameObject.h"

PaintableGameObject::PaintableGameObject(GameObject * g, float objWorth, bool p, Colour col) {
	go = g;
	objectWorth = objWorth;
	isPaintable = p;
	colour = col;
	PhysNodeType::PAINTABLE_OBJECT;
}

void PaintableGameObject::CollisionOccured(ControllableAvatar * p)
{
	//don't do anything if collider already owns the object
	if (p->GetColour() == colour) {
		
	}
	else if (isPaintable) {
		switch (p->GetColour()) {
		case GREEN:
			go->Render()->GetChild()->SetBaseColor(Vector4(0.f, 1.0f, 0.0f, 1.0f));
			colour = GREEN;
			break;

		case RED:
			go->Render()->GetChild()->SetBaseColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			colour = RED;
			break;

		case BLUE:
			go->Render()->GetChild()->SetBaseColor(Vector4(0.f, 0.0f, 1.0f, 1.0f));
			colour = BLUE;
			break;

		case PINK:
			go->Render()->GetChild()->SetBaseColor(Vector4(1.2f, 0.5f, 1.0f, 1.0f));
			colour = PINK;
			break;
		}
	}
}

//void PaintableGameObject::CollisionOccured(Ammo * a) {
//	if (a->GetColour() == colour) {
//
//	}
//	else if (isPaintable) {
//		switch (a->GetOwner()->GetColour()) {
//		case GREEN:
//			go->Render()->GetChild()->SetBaseColor(Vector4(0.f, 1.0f, 0.0f, 1.0f));
//			colour = GREEN;
//			break;
//
//		case RED:
//			go->Render()->GetChild()->SetBaseColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
//			colour = RED;
//			break;
//
//		case BLUE:
//			go->Render()->GetChild()->SetBaseColor(Vector4(0.f, 0.0f, 1.0f, 1.0f));
//			colour = BLUE;
//			break;
//
//		case PINK:
//			go->Render()->GetChild()->SetBaseColor(Vector4(1.2f, 0.5f, 1.0f, 1.0f));
//			colour = PINK;
//			break;
//		}
//	}
//}