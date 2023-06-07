//
// Created by Thalins on 11/24/22.
//

#ifndef ORACLES_TILESET_H
#define ORACLES_TILESET_H

#include <string>
#include <vector>

#include "types.h"

namespace oracle
{
class Tileset
{
public:
	Tileset();
	~Tileset();

	void Generate(const v2u&                      tileSize,
	              const std::vector<std::string>& names,
	              const std::vector<u8>&          data,
	              const v2u&                      atlasSize
	);

	void Bind() const;

	u32                GetTilesetID(const std::string& name) const;
	const std::string& GetTilesetName(u32 id) const;

	const v2u& GetTextureAtlasSize() const;

private:
	std::vector<std::string> _tilesetNames = {};
	v2u                      _tileSize     = v2u::Zero;

	u32 _textureID   = 0;
	v2u _textureSize = v2u::Zero;
};
}

#endif //ORACLES_TILESET_H
