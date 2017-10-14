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
			generateTile(xIndex, yIndex);
		}
	}
}


void Board::generateTile(int x, int y)
{
	bool isTileBlack = shouldTileGenerateBlack(x, y);

	if(y < 2)
	{
		setTileAt(x, y, new Tile(x, y, RED, isTileBlack));
	}
	else if(y > 2)
	{
		setTileAt(x, y, new Tile(x, y, GREEN, isTileBlack));
	}
	else
	{
		if(x <4)
		{
			setTileAt(x, y, new Tile(x, y, GREEN, isTileBlack));
		}
		else if(x > 4)
		{
			setTileAt(x, y, new Tile(x, y, RED, isTileBlack));
		}
		else
		{
			setTileAt(x, y, new Tile(x, y, EMPTY, isTileBlack));
		}

	}
}

bool Board::shouldTileGenerateBlack(int x, int y)
{
	bool xMod2 = (x % 2) == 0;
	bool yMod2 = (y % 2) == 0;

	return (xMod2 && yMod2) || !(xMod2 || yMod2);

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
