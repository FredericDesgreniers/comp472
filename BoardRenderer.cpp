#include "BoardRenderer.h"

BoardRenderer::BoardRenderer(Board board, HDC dcTarget): board(board), dcTarget(dcTarget)
{
	HFONT font = CreateFont(22, 0, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
	SelectObject( dcTarget, font );
}

void BoardRenderer::render(int offSetX, int offSetY)
{
	for(int yPos = 0; yPos < board.getHeight(); yPos++)
	{
		for(int xPos = 0; xPos < board.getWidth(); xPos++)
		{
			Tile *tileToDraw = board.getTileAt(xPos, yPos);
			drawTile(tileToDraw, xPos, yPos, offSetX, offSetY);
		}
	}
}

void BoardRenderer::drawTile(Tile *tile, int x, int y, int offSetX, int offSetY)
{
	char *tileChar = " ";
	if(tile == nullptr)
	{

	}
	else
	{
		tileChar = "n";
	}


	int consolePosX = x * tileWidth + offSetX;
	int consolePosY = y * tileHeight + offSetY;

	if(x % 2 ||  y % 2)
	{
		SetTextColor(dcTarget, 0x00000000);
		SetBkColor(dcTarget, 0x00FFFFFF);
		::Rectangle(dcTarget, consolePosX, consolePosY, consolePosX + tileWidth, consolePosY + tileHeight);

	}
	else
	{
		SetTextColor(dcTarget, 0x00FFFFFF);
		SetBkColor(dcTarget, 0x00000000);
	}



	RECT tileDimension = {consolePosX + 10, consolePosY, consolePosX + tileWidth + 10, consolePosY + tileHeight};
	DrawText(dcTarget, tileChar, 1, &tileDimension, DT_BOTTOM);
}
