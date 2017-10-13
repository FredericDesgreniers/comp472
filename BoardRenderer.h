#pragma once
#include "Board.h"
#include "DrawableBoard.h"

class BoardRenderer
{
protected:
	DrawableBoard drawableBoard;

	virtual void renderStart()
	{

	}

	virtual void renderEnd()
	{

	}

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
		return nullptr;
	}

	virtual int getTilePosXFromDisplayCoordinates(int x)
	{
		return -1;
	}

	virtual int getTilePosYFromDisplayCoordinates(int y)
	{
		return -1;
	}
	DrawableBoard getDrawableBoard(){return drawableBoard;};
};




