#include "Board.h"
#include "TileType.h"
Board::Board(int width, int height):width(width), height(height)
{
	int tileNum = width * height;
	tiles = new Tile*[tileNum];
	for(int i=0; i < tileNum; i++)
	{
		tiles[i] = nullptr;
	}


	for(int yIndex = 0; yIndex < height; yIndex++)
	{
		for(int xIndex = 0; xIndex < width; xIndex++)
		{
			if (xIndex == 0)
			{
				setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, RED));
			}
			else if (xIndex == width-1)
			{
				setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, GREEN));
			}
			else
			{
				setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, EMPTY));
			}
		}
	}
}

Tile *Board::getTileAt(int posX, int posY)
{
	return tiles[posY * width + posX];
}

void Board::setTileAt(int posX, int posY, Tile *tile)
{
	int tileArrayPosition = posY * width + posX;
	if(tiles[tileArrayPosition] != nullptr)
	{
		delete tiles[tileArrayPosition];
	}
	tiles[tileArrayPosition] = tile;
}

int Board::getWidth() const
{
	return width;
}

int Board::getHeight() const
{
	return height;
}

