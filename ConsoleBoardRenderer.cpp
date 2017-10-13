
#include <iostream>
#include "ConsoleBoardRenderer.h"

ConsoleBoardRenderer::ConsoleBoardRenderer(DrawableBoard drawableBoard, HWND handleTarget):BoardRenderer(drawableBoard), handleTarget(handleTarget)
{
	dcTarget = GetDC(handleTarget);
	font = CreateFont(20, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

}

void ConsoleBoardRenderer::drawTile(Tile *tile, int x, int y)
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


	int consolePosX = x * drawableBoard.getTileWidth() + drawableBoard.getX();
	int consolePosY = y * drawableBoard.getTileHeight() + drawableBoard.getY();

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(handleTarget, &pt);
	//std::cout << pt.x << std::endl;

	bool hovered = false;
	if(pt.x > consolePosX && pt.x < consolePosX + drawableBoard.getTileWidth()
			&& pt.y > consolePosY && pt.y < consolePosY + drawableBoard.getTileHeight())
	{
		//std::cout << pt.x << std::endl;
		hovered = true;
	}

	bool xMod2 = x % 2;
	bool yMod2 = y % 2;

	HBRUSH brush;
	if(hovered)
	{
		brush = CreateSolidBrush(RGB(100,100,100));
		SetTextColor(dcTarget, RGB(255, 255, 255));

	}
	else
	if((xMod2 || yMod2) && !(xMod2 && yMod2))
	{

		brush = CreateSolidBrush(RGB(0,0,0));
		SetTextColor(dcTarget, RGB(255, 255, 255));

	}
	else
	{
		brush = CreateSolidBrush(RGB(255,255,255));
		SetTextColor(dcTarget, RGB(0, 0, 0));

	}

	SelectObject(dcTarget, brush);

	::Rectangle(dcTarget, consolePosX, consolePosY, consolePosX + drawableBoard.getTileWidth(), consolePosY + drawableBoard.getTileHeight());

	DeleteObject(brush);


	RECT tileDimension = {consolePosX, consolePosY, consolePosX + drawableBoard.getTileWidth(), consolePosY + drawableBoard.getTileHeight()};


	SetBkMode(dcTarget, TRANSPARENT);
	DrawText(dcTarget, tileChar, 1, &tileDimension, DT_CENTER);
}

void ConsoleBoardRenderer::drawBackground()
{
	int startX = drawableBoard.getX()-1;
	int startY = drawableBoard.getY()-1;
	int endX = startX + drawableBoard.getBoard()->getWidth() * drawableBoard.getTileWidth() + 2;
	int endY = startY + drawableBoard.getBoard()->getHeight() * drawableBoard.getTileHeight() + 2;

	HPEN pen = CreatePen(PS_SOLID, 2, 0x000000FF);

	SelectObject(dcTarget, pen);

	MoveToEx(dcTarget, startX, startY, (LPPOINT)NULL);
	LineTo(dcTarget, startX, endY);
	LineTo(dcTarget, endX, endY);
	LineTo(dcTarget, endX, startY);
	LineTo(dcTarget, startX, startY);
	DeleteObject(pen);
}

bool ConsoleBoardRenderer::isTileHovered(int x, int y)
{



}

