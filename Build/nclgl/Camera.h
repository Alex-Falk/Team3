/******************************************************************************
Class:Camera
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!

-_-_-_-_-_-_-_,------,
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"

#include <ncltech\PhysicsNode.h>

#define ZOOM_SPEED 0.5;

class Camera {
public:
	Camera(void) {
		position = Vector3(0.0f, 0.0f, 0.0f);
		yaw = 0.0f;
		pitch = 0.0f;
		free = true;
		center = nullptr;
		maxDistance = 10.0f;
		minDistance = 1.0;
		distance = (maxDistance + minDistance) / 2;
	};

	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
	}

	~Camera(void) {};

	void HandleMouse(float dt);
	void HandleKeyboard(float dt);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 BuildViewMatrix();
	Vector3 GetViewDirection();

	//Gets position in world space
	inline const Vector3& GetPosition() const { return position; }
	//Sets position in world space
	inline void	SetPosition(const Vector3& val) { position = val; }

	//Gets yaw, in degrees
	inline float GetYaw() const { return yaw; }
	//Sets yaw, in degrees
	inline void SetYaw(float y) { yaw = y; }

	//Gets pitch, in degrees
	inline float	GetPitch() const { return pitch; }
	//Sets pitch, in degrees
	inline void	SetPitch(float p) { pitch = p; }

	//get center
	inline PhysicsNode* GetCenter() const { return center; }
	//set the center
	inline void SetCenter(PhysicsNode* c) { center = c; }

	//get max distance
	inline float GetMaxDistance() const { return maxDistance; }
	//set max distance
	inline void SetMaxDistance(float d) { maxDistance = d; }

	//toggle free
	bool ToggleFree();
protected:
	float	yaw;
	float	pitch;
	Vector3 position;

	//Object which the camera will be centered on
	PhysicsNode* center;
	//maximum and minimum distance from object when not free
	float maxDistance;
	float minDistance;
	//current distance from object
	float distance;


	//is the camera following an object or just flying around
	bool free;
};