#pragma once
#include "GraphicsPipeline.h"
#include <map>


//enum TEXTURETYPE
//{
//	Checker_Board = 0,
//
//	Texture_Number,
//};

class TextureManager : public TSingleton<TextureManager>
{

	friend class TSingleton<TextureManager>;

public :

	bool LoadTexture(TEXTURETYPE type, std::string address, int wrap, int filter);

	bool RemoteTexture(TEXTURETYPE type);

	bool RemoteAllTexture();

	GLuint GetTexture(TEXTURETYPE type);

protected :
	TextureManager();
	virtual ~TextureManager();
	std::map<TEXTURETYPE, GLuint> textures;
	typedef std::map<TEXTURETYPE, GLuint> TextureMap;
};