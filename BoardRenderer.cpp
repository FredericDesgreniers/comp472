
#include "BoardRenderer.h"

BoardRenderer::BoardRenderer(DrawableBoard drawableBoard): drawableBoard(drawableBoard)
{

}

void BoardRenderer::render()
{
	renderStart();

	Board* boardToRender = drawableBoard.getBoard();

	const vec2 boardDimension = boardToRender->getDimension();

	drawBackground();
	for(int yPos = 0; yPos < boardDimension.height; yPos++)
	{
		for(int xPos = 0; xPos < boardDimension.width; xPos++)
		{
			vec2 tilePosition = {xPos, yPos};
			Tile *tileToDraw = boardToRender->getTileAt(tilePosition);
			drawTile(tileToDraw, tilePosition);
		}
	}

	renderEnd();
}


