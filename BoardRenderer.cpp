#include "BoardRenderer.h"

BoardRenderer::BoardRenderer(Board board): board(board)
{

}

void BoardRenderer::render(int offSetX, int offSetY)
{
	drawBackground(offSetX, offSetY);
	for(int yPos = 0; yPos < board.getHeight(); yPos++)
	{
		for(int xPos = 0; xPos < board.getWidth(); xPos++)
		{
			Tile *tileToDraw = board.getTileAt(xPos, yPos);
			drawTile(tileToDraw, xPos, yPos, offSetX, offSetY);
		}
	}
}


