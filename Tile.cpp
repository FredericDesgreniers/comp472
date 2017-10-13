#include "Tile.h"

Tile::Tile(int x, int y, TileType type): x(x), y(y), type(type)
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

