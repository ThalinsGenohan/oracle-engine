#include "Tileset.h"

#include <glad/glad.h>

namespace oracle
{
Tileset::Tileset()
{
	glGenTextures(1, &this->_textureID);
}

Tileset::~Tileset()
{
	glDeleteTextures(1, &this->_textureID);
}

void Tileset::Generate(const v2u& tileSize, const std::vector<std::string>& names, const std::vector<u8>& data,
                       const v2u& atlasSize
)
{
	this->_tileSize    = tileSize;
	this->_textureSize = atlasSize;

	this->_tilesetNames.clear();
	this->_tilesetNames = names;

	glBindTexture(GL_TEXTURE_2D, this->_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_textureSize.X, this->_textureSize.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	             data.data()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Tileset::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
}

u32 Tileset::GetTilesetID(const std::string& name) const
{
	for (u32 i = 0; i < this->_tilesetNames.size(); i++)
	{
		if (this->_tilesetNames[i] == name) return i;
	}
	return 0;
}

const std::string& Tileset::GetTilesetName(const u32 id) const
{
	return this->_tilesetNames[id];
}

const v2u& Tileset::GetTextureAtlasSize() const
{
	return this->_textureSize;
}
}
