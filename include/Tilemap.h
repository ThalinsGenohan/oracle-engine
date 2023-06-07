//
// Created by Thalins on 11/1/22.
//

#ifndef ORACLES_TILEMAP_H
#define ORACLES_TILEMAP_H

#include <vector>

#include "types.h"

namespace oracle
{
class Tileset;

class Tilemap
{
public:
	Tilemap();

	~Tilemap();

	void Delete();

	const v2u& GetMapSize() const;
	void       SetMapSize(const v2u& size);
	void       SetMapSize(u32 width, u32 height);

	const v2i& GetPosition() const;
	void       SetPosition(v2i pos);
	void       SetPosition(i32 posX, i32 posY);

	const std::vector<u16>& GetTiles();
	u32                     GetTileCount() const;

	void SetTiles(const std::vector<u16>& tiles);
	void SetTiles(const u16* tiles);

	void SetTile(const v2u& coord, u16 newTile);
	void SetTile(u32 x, u32 y, u16 newTile);
	void SetTile(u32 index, u16 newTile);

	u32  GetTilesetID() const;
	void SetTilesetID(u32 id);

	bool IsTileUpdatePending() const;
	void SetTileUpdate(bool update);

private:
	v2u _mapSize  = v2u::Zero;
	v2i _position = v2i::Zero;

	bool             _tileUpdatePending = false;
	std::vector<u16> _tiles             = {};

	u32 _tileset = 0;
};
}

#endif //ORACLES_TILEMAP_H
