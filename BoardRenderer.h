#pragma once

#include "GameMemory.h"

class BoardRenderer
{
protected:
	TileType* tilesToDraw;
	vec2 tileBoardDimension;
	vec2 pixelDimension = {0, 0};
	vec2 tileDimension = {50,50};

	virtual void renderStart()
	{

	}

	virtual void renderEnd()
	{

	}

	virtual void drawTile(vec2 position, TileType  type)
	{

	}

	virtual void drawBackground()
	{

	}


public:
	BoardRenderer(TileType* tilesToDraw, vec2 tileBoardDimension);

	void render();


	virtual const vec2 getTilePositionFromDisplayPosition(const vec2 position)
	{
		return {-1, -1};
	}
};




