#include "Tile.h"

Tile::Tile(int x, int y, bool isEmpty): x(x), y(y), isEmpty(isEmpty)
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

