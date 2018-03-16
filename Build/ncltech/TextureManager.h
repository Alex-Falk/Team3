
/*****************************************************************************
:;-"""-::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
(      .)::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
        )-:::::::::::::::::::::::::::::::::::::::::::::::.::..:... .. .
      -'   )-"-:::::::::::::::::::::::::::::::::::::::.::..:... ..  .
 ___________)______________          _____
|                         \          \ U \__      _______
|        Yesheng Su        \__________\   \/_______\___\_____________
|        10/02/2018        /          < /_/   ..................... ^`-._
|_________________________/            `-----------,----,--------------'
                      )                          _/____/_
-.                .    ):::::::::::::::::::::::::::::.::..:... ..  .
  )--.__..--"-.__,'---':::::::::::::::::::::::::::::::::.::..:... ..  .
-':::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
:::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
															
*****************************************************************************/

/*****************************************************************************
:;-"""-::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
(      .)::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
        )-:::::::::::::::::::::::::::::::::::::::::::::::.::..:... .. .
      -'   )-"-:::::::::::::::::::::::::::::::::::::::.::..:... ..  .
 ___________)______________          _____
|                         \          \ U \__      _______
|        Yesheng Su        \__________\   \/_______\___\_____________
|        14/02/2018        /          < /_/   ..................... ^`-._
|_________________________/            `-----------,----,--------------'
                      )                          _/____/_
-.                .    ):::::::::::::::::::::::::::::.::..:... ..  .
  )--.__..--"-.__,'---':::::::::::::::::::::::::::::::::.::..:... ..  .
-':::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
:::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
															
*****************************************************************************/
#pragma once
#include "GraphicsPipeline.h"
#include <map>


enum TEXTURETYPE
{
	Ground_Texture		= 0,
	Sky_Box				= 1,
	//minimap stuff (phil 26/02/2018)
	Paint_Pool			= 2,
	Gun_Pickup			= 3,
	Speed_Pickup		= 4,
	Jump_Pickup			= 5,
	Capture_Point		= 6,
	Change_Color_Mask	= 7,
	WaterDUDV			= 8,
	Water				= 9,
	Avatartex			= 10,
	//Map Textures for Data Driven Map. Don't Change the order. Nikos Fragkas
	Item_Texture_1,
	Item_Texture_2,
	Item_Texture_3,
	Item_Texture_4,
	Item_Texture_5,
	Item_Texture_6,
	Item_Texture_7,
	Item_Texture_8,
	Item_Texture_9,
	Item_Texture_10,
	// -----------------------------------------------------------------------
	Texture_Number,
};

class TextureManager : public TSingleton<TextureManager>
{

	friend class TSingleton<TextureManager>;

public :

	bool LoadTexture(TEXTURETYPE type, std::string address, int wrap, int filter);

	bool LoadCubeMap(TEXTURETYPE type, std::string address_XPos, std::string address_XNeg, std::string address_YPos, std::string address_YNeg, std::string address_ZPos, std::string address_ZNeg );

	bool RemoteTexture(TEXTURETYPE type);

	bool RemoveAllTexture();

	GLuint GetTexture(TEXTURETYPE type);

protected :
	TextureManager();
	virtual ~TextureManager();
	std::map<TEXTURETYPE, GLuint> textures;
	typedef std::map<TEXTURETYPE, GLuint> TextureMap;
};