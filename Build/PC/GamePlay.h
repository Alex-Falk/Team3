#pragma once

#include <nclgl\Vector4.h>

#define DEFAULT_COLOUR Vector4(0.5f, 0.5f, 0.5f, 1.0f)
#define GREEN_COLOUR Vector4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE_COLOUR Vector4(0.0f, 0.0f, 1.0f, 1.0f)
#define RED_COLOUR Vector4(1.0f, 0.0f, 0.0f, 1.0f)
#define PINK_COLOUR Vector4(1.0f, 0.0f, 1.0f, 1.0f)

enum Colour
{
	RED, GREEN, BLUE, PINK, START_COLOUR //Keep Start colour always at the end
};

//Name: Nick Bedford
//Date: 06/02/2018
//Added PickupType
enum PickupType 
{
	SPEED_BOOST, JUMP_BOOST, WEAPON, PAINTPOOL
};

//Name: Nick Bedford
//Date: 07/02/2018
//Added WeaponType
enum WeaponType
{
	PAINT_SPRAY, PAINT_PISTOL, AUTO_PAINT_LAUNCHER, PAINT_ROCKET, NUM_OF_WEAPONS
};

//Name: Nick Bedford
//Date: 09/03/2018
//Added CaptureAreaType
enum CaptureAreaType
{
	DEFAULT_CAPTURE_AREA, MULTIPAINTPOOL_CAPTURE_AREA, MINION_CAPTURE_AREA, NUM_OF_CAPTURE_AREA_TYPES
};

//Game music should always be the last music. So if you add more music, put it before Game music
enum Audio {
	MENU_MUSIC, GAME_MUSIC, JUMP_SOUND, ROCKET_FLYING_SOUND, EXPLOSION_SOUND, PROJECTILE_LAUNCH_SOUND, MENU_CHOICE_SOUND, PICKUP_COLLECTED_SOUND, VICTORY_SOUND, LOSS_SOUND, CAPTURE_AREA_SOUND, PAINT_FILL_SOUND, PROJECTILE_HIT_SOUND
};

//Function to get the vector 4 colour of an enum 
static Vector4 EnumToVectorColour(Colour c)
{
	switch (c)
	{
		case RED:			return RED_COLOUR;
		case GREEN:			return GREEN_COLOUR;
		case BLUE:			return BLUE_COLOUR;
		case PINK:			return PINK_COLOUR;
		default:			return DEFAULT_COLOUR;
	}
}