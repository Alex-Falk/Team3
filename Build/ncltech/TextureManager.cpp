#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	RemoveAllTexture();
}

bool TextureManager::LoadTexture(TEXTURETYPE type, std::string address, int wrap, int filter)
{
	TextureMap::iterator it = textures.find(type);
	if (it != textures.end())
		return false; // alreadly have texture

	GLuint tex = SOIL_load_OGL_texture(address.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);

	if (tex)
	{
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); //No linear interpolation to get crisp checkerboard no matter the scalling
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		return false; // create texture failed

	textures[type] = tex;

	return true; // create texture succeed
}

bool TextureManager::RemoteTexture(TEXTURETYPE type)
{
	TextureMap::iterator it = textures.find(type);
	if (it == textures.end())
		return false; // dont have texture

	glDeleteTextures(1,&(*it).second);
	textures.erase(it);

	return true; // remote succeed
}

bool TextureManager::RemoveAllTexture()
{
	for (TextureMap::iterator i = textures.begin(); i != textures.end(); ++i)
	{
		glDeleteTextures(1, &(*i).second);
	}

	textures.clear();

	return true;
}

GLuint TextureManager::GetTexture(TEXTURETYPE type)
{
	TextureMap::iterator it = textures.find(type);
	if (it == textures.end())
		return 0; // dont have texture

	return (*it).second;
}


