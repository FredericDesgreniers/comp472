#include "Tile.h"

Tile::Tile(int x, int y, TileType type, bool isBlack): x(x), y(y), type(type), isBlack(isBlack)
{

}

int Tile::getX() const
{
	return x;
}

int Tile::getY() const
{
	return y;
}

