#pragma once


#include "Tile.h"

class Board
{
private:
	Tile** tiles;
	int width;
	int height;
public:
	Board(int width, int height);

	Tile* getTileAt(int posX, int posY);

	void setTileAt(int posX, int posY, Tile* tile);

	int getWidth() const;

	int getHeight() const;


};




