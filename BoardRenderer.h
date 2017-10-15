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

	virtual void drawTile(Tile* tile, const vec2 position)
	{

	}

	virtual void drawBackground()
	{

	}


public:
	BoardRenderer(DrawableBoard drawableBoard);

	void render();

	virtual Tile* getTileAtDisplayCoordinates(const vec2 position)
	{
		return nullptr;
	}

	virtual const vec2 getTilePositionFromDisplayPosition(const vec2 position)
	{
		return {-1, -1};
	}

	DrawableBoard getDrawableBoard(){return drawableBoard;};
};




