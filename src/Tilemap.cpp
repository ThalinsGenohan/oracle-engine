//
// Created by Thalins on 11/1/22.
//
#include "Tilemap.h"

#include "Game.h"

namespace oracle
{
Tilemap::Tilemap() {}

Tilemap::~Tilemap()
{
	_tiles.clear();
}

void Tilemap::Delete()
{
	_tiles.clear();
}

const v2u& Tilemap::GetMapSize() const
{
	return this->_mapSize;
}

void Tilemap::SetMapSize(const v2u& size)
{
	this->_mapSize = size;
}

void Tilemap::SetMapSize(const u32 width, const u32 height)
{
	this->_mapSize = v2u(width, height);
}

const v2i& Tilemap::GetPosition() const
{
	return this->_position;
}

void Tilemap::SetPosition(const v2i pos)
{
	this->_position = pos;
}

void Tilemap::SetPosition(const i32 posX, const i32 posY)
{
	this->SetPosition(v2i(posX, posY));
}

const std::vector<u16>& Tilemap::GetTiles()
{
	return this->_tiles;
}

u32 Tilemap::GetTileCount() const
{
	return this->_tiles.size();
}

void Tilemap::SetTiles(const std::vector<u16>& tiles)
{
	this->_tiles = tiles;
}

void Tilemap::SetTiles(const u16* tiles)
{
	this->_tiles = std::vector<u16>(static_cast<size_t>(this->_mapSize.Y) * static_cast<size_t>(this->_mapSize.X));
	for (u32 i = 0; i < this->_mapSize.Y * this->_mapSize.X; i++)
	{
		this->_tiles[i] = tiles[i];
	}
}

void Tilemap::SetTile(const v2u& coord, const u16 newTile)
{
	this->SetTile(coord.X, coord.Y, newTile);
}

void Tilemap::SetTile(const u32 x, const u32 y, const u16 newTile)
{
	this->SetTile(y * this->_mapSize.X + x, newTile);
}

void Tilemap::SetTile(const u32 index, const u16 newTile)
{
	this->_tiles[index] = newTile;
	this->SetTileUpdate(true);
}

u32 Tilemap::GetTilesetID() const
{
	return this->_tileset;
}

void Tilemap::SetTilesetID(const u32 id)
{
	this->_tileset = id;
}

bool Tilemap::IsTileUpdatePending() const
{
	return this->_tileUpdatePending;
}

void Tilemap::SetTileUpdate(const bool update)
{
	this->_tileUpdatePending = update;
}
}
