#include "Board.h"

Board::Board(int width, int height):width(width), height(height)
{
	int tileNum = width * height;
	tiles = new Tile*[tileNum];

	for(int tileIndex = 0; tileIndex < tileNum; tileIndex++)
	{
		tiles[tileIndex] = nullptr;
	}

	tiles[0] = new Tile(0, 0);
}

Tile *Board::getTileAt(int posX, int posY)
{
	return tiles[posY * width + posX];
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
