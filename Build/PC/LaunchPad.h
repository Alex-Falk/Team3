/*
           \\\///
	      / ~  ~ \
		(| (.)(.) |)
.-----.OOOo--C---oOOO.---.
|                        |
|   Made By Philip Beck  |
|      14/03/2018        |
|                        |
'----.oooO---------------'
     (   )           Oooo.
      \ (	    	 (	  )
	   \_)      	  ) /
			         (_/
*/

#pragma once
#ifndef LAUNCHPAD_H
#define LAUNCHPAD_H

#include <ncltech\GameObject.h>

class LaunchPad : public GameObject {
public:
	LaunchPad(Vector3 pos, Vector3 halfDims, string unique_name, float power = 80);
	LaunchPad() :LaunchPad(Vector3(0,0,0),Vector3(2,1,2), "pad"){};
	~LaunchPad() {};
	bool LaunchPadCallbackFunction(PhysicsNode* self, PhysicsNode* collidingObject);
	void Update(float dt);
private:
	float power;
	float s;
};

#endif