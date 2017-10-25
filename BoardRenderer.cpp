
#include "BoardRenderer.h"

BoardRenderer::BoardRenderer(TileType*tilesToDraw, vec2 tileBoardDimension): tilesToDraw(tilesToDraw), tileBoardDimension(tileBoardDimension), pixelDimension(tileDimension.width * tileBoardDimension.width, tileDimension.height * tileBoardDimension.height)
{
	pixelDimension = {tileDimension.width * tileBoardDimension.width, tileDimension.height * tileBoardDimension.height};
}

void BoardRenderer::render()
{
	renderStart();

	drawBackground();
	for(int yPos = 0; yPos < tileBoardDimension.height; yPos++)
	{
		for(int xPos = 0; xPos < tileBoardDimension.width; xPos++)
		{
			TileType typeToDraw = tilesToDraw[yPos * tileBoardDimension.width + xPos];
			drawTile({xPos, yPos}, typeToDraw);
		}
	}

	renderEnd();
}


