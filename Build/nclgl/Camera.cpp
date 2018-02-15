#include "Camera.h"

#include <ncltech\PhysicsEngine.h>
#include <ncltech\CollisionDetectionSAT.h>
#include <ncltech\GameObject.h>

#include "..\PC\GameInput.h"

//I blame Microsoft...
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

//the minimum time before the turning starts
#define MIN_TURN_TIME 1.0
//how slowly the tunrning speed increases (larger increases slower)
#define TURN_INCREASE_RATE 30
//minimum and maximum turn speed in degrees per second
#define MIN_TURN_SPEED 0.03
#define MAX_TURN_SPEED 0.25
//how fast the player must be moving for the camera to rotate
#define MIN_CENTER_SPEED_SQUARED 1.0



//Added by phil
//07/02/2018
//combines the old handle mouse and handle keyboard functions
//and makes the camera follow the ball direction
void Camera::UpdateCamara(float dt) {
	//used to be handle mouse:
	//Update the mouse by how much
	if (Window::GetMouse()->ButtonDown(MOUSE_LEFT) || !free)
	{
		pitch -= (Input::Instance()->GetLookY());
		yaw -= (Input::Instance()->GetLookX());
		//if the mouse hasn't moved in the x add time onto time since mouse
		if (Input::Instance()->GetLookX() == 0) {
			timeSinceMouse += dt;
		}
		else {
			timeSinceMouse = 0;
		}
	}

	//float wheel_speed = Window::GetMouse()->GetWheelMovement() * 0.5f;
	//Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	//position += rotation * Vector3(0, 0, -1) * wheel_speed;


	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = (float)min(pitch, free?90.0:25.0f);//capped at 7 instead of 90 to stop the floor making the spring arm jerky
	pitch = (float)max(pitch, -89.0f);

	if (yaw < 0.0f) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	//The artist formally known as handle keyboard:
	float speed = 3.5f * dt;	//3.5m per second


								//Bounds check the pitch, to be between straight up and straight down ;)
	if (yaw <0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}


	if (Input::Instance()->GetInput(FORWARD)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * speed;
	}
	if (Input::Instance()->GetInput(BACKWARD)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * speed;
	}

	if (Input::Instance()->GetInput(LEFT)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * speed;
	}
	if (Input::Instance()->GetInput(RIGHT)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * speed;
	}

	if (Input::Instance()->GetInput(CAMERA_UP)) {
		position.y += speed;
	}
	if (Input::Instance()->GetInput(CAMERA_DOWN)) {
		position.y -= speed;
	}

	//NEW STUFF!!!
	float x = center->GetLinearVelocity().x;
	float z = center->GetLinearVelocity().z;
	//There are a lot of magic numbers here these should be dealt with
	if (timeSinceMouse > MIN_TURN_TIME && (x*x + z*z) > MIN_CENTER_SPEED_SQUARED) {
		//the camera moves itself faster the longer you haven't moved it
		float turnSpeed = (float)((timeSinceMouse - MIN_TURN_TIME)/ TURN_INCREASE_RATE);
		turnSpeed = (float)max(turnSpeed, MIN_TURN_SPEED);
		turnSpeed = (float)min(turnSpeed, MAX_TURN_SPEED);
		//get the direction of the x and y vector and have the yaw approach it


		//both are now angles between 0 and 360
		float angle = (180 * atan2(x, z)/PI) + 180.0f;
		//find the difference between the two angles
		int diff = (int)(angle - yaw);
		diff = (diff + 180) % 360 - 180;

		if (diff < -1) {
			yaw -= turnSpeed;
		}
		if (diff > 1) {
			yaw += turnSpeed;
		}
	}


	//update the distance
	UpdateDistance();
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix() {
	if (!free && center != nullptr) {
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

//michael davis 08/02/2018, needed for audio
Vector3 Camera::GetUpDirection() {
	return Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0)) * Vector3(0, 1, 0);
}



bool Camera::ToggleFree() {
	//toggle the value for free movement if there is a center object
	if (center) {
		free = !free;
	}

	return free;
}

void Camera::UpdateDistance() {
	//don't do anything if the camera isn't free
	if (!free) {
		//testing
		LineCollision d = PhysicsEngine::Instance()->CastRay(center->GetPosition() - GetViewDirection() * center->GetBoundingRadius(), -GetViewDirection(), center);
		if (d.node) {
			distance = (d.dist < maxDistance) ? d.dist : maxDistance;
		}
		else {
			distance = maxDistance;
		}
	}
}