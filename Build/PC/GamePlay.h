#pragma once

// Level dimensions
#define DIMENSION_X (80.0f)
#define DIMENSION_Y (80.0f)

#define DEFAULT_COLOUR Vector4(0.5f, 0.5f, 0.5f, 1.0f)
#define GREEN_COLOUR Vector4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE_COLOUR Vector4(0.0f, 0.0f, 1.0f, 1.0f)
#define RED_COLOUR Vector4(1.0f, 0.0f, 0.0f, 1.0f)
#define PINK_COLOUR Vector4(1.0f, 0.2f, 1.0f, 1.0f)


enum Colour
{
	RED, GREEN, BLUE, PINK, START_COLOUR
};

//Name: Nick Bedford
//Date: 06/02/2018
//Added PickupType
enum PickupType 
{
	SPEED_BOOST, JUMP_BOOST, WEAPON
};

//Name: Nick Bedford
//Date: 07/02/2018
//Added WeaponType
enum WeaponType
{
	PAINT_SPRAY, PAINT_PISTOL, AUTO_PAINT_LAUNCHER, PAINT_ROCKET, NUM_OF_WEAPONS
};

//Game music should always be the last music. So if you add more music, put it before Game music
enum Audio {
	MENU_MUSIC, GAME_MUSIC, JUMP_SOUND
};