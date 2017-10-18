#include "Board.h"
#include "TileType.h"

Board::Board(const vec2 dimension):dimension(dimension)
{
	int tileNum = getArea(dimension);

	tiles = new Tile*[tileNum]{nullptr};


	for(int yIndex = 0; yIndex < dimension.height; yIndex++)
	{
		for(int xIndex = 0; xIndex < dimension.width; xIndex++)
		{
			generateTile(vec2{xIndex, yIndex});
		}
	}
}


void Board::generateTile(const vec2 position)
{
	bool isTileBlack = shouldTileGenerateBlack(position);

	if(position.y < 2)
	{
		setTileAt(position, new Tile(position, RED, isTileBlack));
	}
	else if(position.y > 2)
	{
		setTileAt(position, new Tile(position, GREEN, isTileBlack));
	}
	else
	{
		if(position.x <4)
		{
			setTileAt(position, new Tile(position, GREEN, isTileBlack));
		}
		else if(position.x > 4)
		{
			setTileAt(position, new Tile(position, RED, isTileBlack));
		}
		else
		{
			setTileAt(position, new Tile(position, EMPTY, isTileBlack));
		}

	}
}

bool Board::shouldTileGenerateBlack(const vec2 position)
{
	bool xMod2 = (position.x % 2) == 0;
	bool yMod2 = (position.y % 2) == 0;

	return (xMod2 && yMod2) || !(xMod2 || yMod2);

}

Tile *Board::getTileAt(const vec2 position)
{
	if(position.x < 0 || position.y < 0 || position.x >= dimension.width || position.y >= dimension.height)
	{
		return nullptr;
	}
	return tiles[position.y * dimension.width + position.x];
}

void Board::setTileAt(const vec2 position, Tile *tile)
{
	int tileArrayPosition = position.y * dimension.width + position.x;
	if(tiles[tileArrayPosition] != nullptr)
	{
		//delete tiles[tileArrayPosition];
	}
	tiles[tileArrayPosition] = tile;
}