#include "Board.h"

Board::Board(int width, int height):width(width), height(height)
{
	int tileNum = width * height;
	tiles = new Tile*[tileNum];


	for(int yIndex = 0; yIndex < height; yIndex++)
	{
		for(int xIndex = 0; xIndex < width; xIndex++)
		{
			if (xIndex == 0)
			{
				setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, false));
			} else
			{
				setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, true));
			}
		}
	}

	for(int tileIndex = 0; tileIndex < tileNum; tileIndex++)
	{

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

void Board::setWidth(int width)
{
	Board::width = width;
}

int Board::getHeight() const
{
	return height;
}

void Board::setHeight(int height)
{
	Board::height = height;
}

