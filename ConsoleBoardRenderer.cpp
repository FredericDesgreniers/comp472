
#include "ConsoleBoardRenderer.h"

ConsoleBoardRenderer::ConsoleBoardRenderer(Board board, HDC dcTarget):BoardRenderer(board), dcTarget(dcTarget)
{
	font = CreateFont(20, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

}

void ConsoleBoardRenderer::drawTile(Tile *tile, int x, int y, int offSetX, int offSetY)
{
	SelectObject( dcTarget, font );
	char *tileChar = "a";
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
		SetTextColor(dcTarget, RGB(255,255,255));
		SetBkColor(dcTarget, RGB(0,0,0));

		HPEN pen = CreatePen(PS_SOLID, 2, RGB(255,255,255));
		HBRUSH brush = CreateSolidBrush(0x00FFFFFF);

		SelectObject(dcTarget, pen);
		SelectObject(dcTarget, brush);

		::Rectangle(dcTarget, consolePosX, consolePosY, consolePosX + tileWidth, consolePosY + tileHeight);
		DeleteObject(pen);
	}
	else
	{
		SetTextColor(dcTarget, RGB(0,0,0));
		SetBkColor(dcTarget, RGB(255,255,255));

		HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));

		SelectObject(dcTarget, pen);
		::Rectangle(dcTarget, consolePosX, consolePosY, consolePosX + tileWidth, consolePosY + tileHeight);
		DeleteObject(pen);
	}



	RECT tileDimension = {consolePosX, consolePosY, consolePosX + tileWidth, consolePosY + tileHeight};
	DrawText(dcTarget, tileChar, 1, &tileDimension, DT_BOTTOM);
}

void ConsoleBoardRenderer::drawBackground(int offSetX, int offsetY)
{
	int startX = offSetX-1;
	int startY = offsetY-1;
	int endX = startX + board.getWidth() * tileWidth+2;
	int endY = startY + board.getHeight() * tileHeight+2;

	HPEN pen = CreatePen(PS_SOLID, 2, 0x000000FF);

	SelectObject(dcTarget, pen);

	MoveToEx(dcTarget, startX, startY, (LPPOINT)NULL);
	LineTo(dcTarget, startX, endY);
	LineTo(dcTarget, endX, endY);
	LineTo(dcTarget, endX, startY);
	LineTo(dcTarget, startX, startY);
	DeleteObject(pen);
}

