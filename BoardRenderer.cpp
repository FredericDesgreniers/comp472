#include <windef.h>
#include <pbt.h>
#include "BoardRenderer.h"

BoardRenderer::BoardRenderer(DrawableBoard drawableBoard): drawableBoard(drawableBoard)
{

}

void BoardRenderer::render()
{
	renderStart();

	Board* boardToRender = drawableBoard.getBoard();

	drawBackground();
	for(int yPos = 0; yPos < boardToRender->getHeight(); yPos++)
	{
		for(int xPos = 0; xPos < boardToRender->getWidth(); xPos++)
		{
			Tile *tileToDraw = boardToRender->getTileAt(xPos, yPos);
			drawTile(tileToDraw, xPos, yPos);
		}
	}

	renderEnd();
}


