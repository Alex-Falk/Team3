#include "Camera.h"

#include <ncltech\PhysicsEngine.h>
#include <ncltech\CollisionDetectionSAT.h>

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::HandleMouse(float dt)
{

	//Update the mouse by how much
	if (Window::GetMouse()->ButtonDown(MOUSE_LEFT) || !free)
	{
		pitch -= (Window::GetMouse()->GetRelativePosition().y);
		yaw -= (Window::GetMouse()->GetRelativePosition().x);
	}

	//float wheel_speed = Window::GetMouse()->GetWheelMovement() * 0.5f;
	//Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	//position += rotation * Vector3(0, 0, -1) * wheel_speed;


	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch, 7.0f);//capped at 7 instead of 90 to stop the floor making the spring arm jerky
	pitch = max(pitch, -90.0f);

	if (yaw < 0.0f) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
}

void Camera::HandleKeyboard(float dt)
{
	float speed = 3.5f * dt;	//3.5m per second


								//Bounds check the pitch, to be between straight up and straight down ;)
	if (yaw <0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}


	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= speed;
	}
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() {
	if (!free) {
		position = center->GetPosition();
		position -= GetViewDirection() * distance * center->GetBoundingRadius();
	}
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};

Vector3 Camera::GetViewDirection() {
	return Matrix3::Rotation(pitch, Vector3(1, 0, 0)) * Matrix3::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1);
}


bool Camera::ToggleFree() {
	//toggle the value for free movement if there is a center object
	if (center) {
		free = !free;
	}

	return free;
}

void Camera::UpdateDistance() {

	//put the arm in the right place
	arm.SetOrientation(Quaternion::FromMatrix(Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0))));
	//the center of the arm it starts at the edge of the centered object so it doesn't collide with that
	Vector3 armPos = center->GetPosition() + (GetViewDirection() * -(maxDistance + center->GetBoundingRadius())) / 2;
	arm.SetPosition(armPos);
	//don't do anything if the camera isn't free
	if (!free) {
		//Collision Detection Algorithm to use
		CollisionDetectionSAT colDetect;
		Manifold manifold;

		//set the distance to the maximum to start with
		distance = maxDistance;
		//cast a ray and check for collisions
		for (PhysicsNode* node : PhysicsEngine::Instance()->physicsNodes) {
			//if there is a collision move the camera to that point
			//broadPhase
			if ((arm.GetPosition() - node->GetPosition()).LengthSQ() < pow(arm.GetBoundingRadius() + node->GetBoundingRadius(), 2)) {
				//narrow
				colDetect.BeginNewPair(&arm, node, arm.GetCollisionShape(), node->GetCollisionShape());

				//if there is a collision
				if (colDetect.AreColliding()) {
					//use the new shape
					manifold.Initiate(&arm, node);

					//make contact points
					colDetect.GenContactPoints(&manifold);

					//the new distance is the length of the nearest contanct point
					for (ContactPoint p : manifold.contactPoints) {
						float newDist = ((arm.GetPosition() + p.relPosA) - center->GetPosition()).Length();
						if (newDist < distance) {
							distance = newDist;
						}
					}
					
					//if the minimum distance has been reached end the checking
					if (distance <= minDistance) {
						distance = minDistance;
						return;
					}
				}
			}
		}
	}
}

void Camera::ResizeArm() {
	float r = center->GetBoundingRadius();
	armShape->SetHalfDepth((maxDistance*r / 2) - r);
	arm.SetBoundingRadius(armShape->GetHalfDepth());
}