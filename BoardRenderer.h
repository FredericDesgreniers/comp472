#pragma once
#include "Board.h"

class BoardRenderer
{
protected:
	Board board;

	int tileWidth = 20;
	int tileHeight = 20;

	virtual void drawTile(Tile* tile, int x, int y, int offSetX, int offSetY)
	{

	}
public:
	BoardRenderer(Board board);

	void render(int offSetX, int offSetY);
};




