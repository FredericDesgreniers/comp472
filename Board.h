#pragma once


#include "Tile.h"

class Board
{
private:
	Tile** tiles;
	const vec2 dimension;

	void generateTile(const vec2 position);

	bool shouldTileGenerateBlack(const vec2 position);

public:
	Board(const vec2 dimension);

	Tile* getTileAt(const vec2 position);

	void setTileAt(const vec2 position, Tile* tile);

	const vec2 getDimension(){return dimension;}
};




