#pragma once
#include "Board.h"
#include "DrawableBoard.h"

class BoardRenderer
{
protected:
	DrawableBoard drawableBoard;

	virtual void drawTile(Tile* tile, int x, int y)
	{

	}

	virtual void drawBackground()
	{

	}


public:
	BoardRenderer(DrawableBoard drawableBoard);

	void render();

	virtual Tile* getTileAtDisplayCoordinates(int x, int y)
	{

	}
};




