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

			bool isTileBlack = shouldTileGenerateBlack(xIndex, yIndex);

			if(yIndex < 2)
			{
				setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, RED, isTileBlack));
			}
			else if(yIndex > 2)
			{
				setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, GREEN, isTileBlack));
			}
			else
			{
				if(xIndex <4)
				{
					setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, GREEN, isTileBlack));
				}
				else if(xIndex > 4)
				{
					setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, RED, isTileBlack));
				}
				else
				{
					setTileAt(xIndex, yIndex, new Tile(xIndex, yIndex, EMPTY, isTileBlack));
				}

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

bool Board::shouldTileGenerateBlack(int x, int y)
{
	bool xMod2 = (x % 2) == 0;
	bool yMod2 = (y % 2) == 0;

	if((xMod2 && yMod2) || (!xMod2 && !yMod2))
	{
		return true;
	}
	else
	{
		return false;
	}

}

