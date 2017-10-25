
#include <cstring>
#include "GameMemory.h"

bool GameMemory::isPositionOnBoard(vec2 position)
{
	return position.x >= 0 && position.x < BOARD_WIDTH &&
			position.y >= 0 && position.y < BOARD_HEIGHT;
}

TileType GameMemory::getTileAt(vec2 position)
{
	if(!isPositionOnBoard(position))
	{
		return INVALID;
	}
	return tiles[position.y][position.x];
}

void GameMemory::setTileAt(vec2 position, TileType type)
{
	if(isPositionOnBoard(position))
	{
		tiles[position.y][position.x] = type;
	}
}

GameMemory::GameMemory()
{
	memset(tiles, getArea({9, 5}), 1);

	for(int yIndex = 0; yIndex < 5; yIndex++)
	{
		for(int xIndex = 0; xIndex < 9; xIndex++)
		{
			generateTile(vec2{xIndex, yIndex});
		}
	}
}

void GameMemory::generateTile(vec2 position)
{
	if(position.y < 2)
	{
		setTileAt(position, RED);
	}
	else if(position.y > 2)
	{
		setTileAt(position, GREEN);
	}
	else
	{
		if(position.x <4)
		{
			setTileAt(position, GREEN);
		}
		else if(position.x > 4)
		{
			setTileAt(position, RED);
		}
		else
		{
			setTileAt(position, EMPTY);
		}

	}
}
