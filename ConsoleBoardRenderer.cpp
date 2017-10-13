
#include <iostream>
#include "ConsoleBoardRenderer.h"

ConsoleBoardRenderer::ConsoleBoardRenderer(DrawableBoard drawableBoard, HWND handleTarget):BoardRenderer(drawableBoard), handleTarget(handleTarget)
{
	dcTarget = GetDC(handleTarget);

	font = CreateFont(30, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

}

void ConsoleBoardRenderer::drawTile(Tile *tile, int x, int y)
{
	SelectObject( dcBufferTarget, font );
	char *tileChar = "a";
	if(tile == nullptr)
	{

	}
	else
	{
		tileChar = "n";
	}


	int consolePosX = x * drawableBoard.getTileWidth() + 1;
	int consolePosY = y * drawableBoard.getTileHeight() + 1;

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
	if(hovered || (tile != nullptr && tile->isSelected))
	{
		brush = CreateSolidBrush(RGB(100,100,100));
		SetTextColor(dcBufferTarget, RGB(255, 255, 255));

	}
	else
	if((xMod2 || yMod2) && !(xMod2 && yMod2))
	{

		brush = CreateSolidBrush(RGB(0,0,0));
		SetTextColor(dcBufferTarget, RGB(255, 255, 255));

	}
	else
	{
		brush = CreateSolidBrush(RGB(255,255,255));
		SetTextColor(dcBufferTarget, RGB(0, 0, 0));

	}

	SelectObject(dcBufferTarget, brush);

	::Rectangle(dcBufferTarget, consolePosX, consolePosY, consolePosX + drawableBoard.getTileWidth(), consolePosY + drawableBoard.getTileHeight());

	DeleteObject(brush);


	RECT tileDimension = {consolePosX, consolePosY, consolePosX + drawableBoard.getTileWidth(), consolePosY + drawableBoard.getTileHeight()};


	SetBkMode(dcBufferTarget, TRANSPARENT);
	DrawText(dcBufferTarget, tileChar, 1, &tileDimension, DT_CENTER);
}

void ConsoleBoardRenderer::drawBackground()
{
	int startX = 0;
	int startY = 0;
	int endX = startX + drawableBoard.getBoard()->getWidth() * drawableBoard.getTileWidth() + 2;
	int endY = startY + drawableBoard.getBoard()->getHeight() * drawableBoard.getTileHeight() + 2;

	HPEN pen = CreatePen(PS_SOLID, 2, 0x000000FF);

	SelectObject(dcBufferTarget, pen);

	MoveToEx(dcBufferTarget, startX, startY, (LPPOINT)NULL);
	LineTo(dcBufferTarget, startX, endY);
	LineTo(dcBufferTarget, endX, endY);
	LineTo(dcBufferTarget, endX, startY);
	LineTo(dcBufferTarget, startX, startY);
	DeleteObject(pen);
}

Tile *ConsoleBoardRenderer::getTileAtDisplayCoordinates(int x, int y)
{
	int relativeX = x - drawableBoard.getX();
	int relativeY = y - drawableBoard.getY();

	if(relativeX > 0 && relativeX < drawableBoard.getTileWidth() * drawableBoard.getBoard()->getWidth() &&
			relativeY > 0 && relativeY < drawableBoard.getTileHeight() * drawableBoard.getBoard()->getHeight())
	{
		int tilePosX = relativeX / drawableBoard.getTileWidth();
		int tilePosY = relativeY / drawableBoard.getTileHeight();

		return drawableBoard.getBoard()->getTileAt(tilePosX, tilePosY);
	}

	return nullptr;
}

void ConsoleBoardRenderer::renderStart()
{
	dcBufferTarget = CreateCompatibleDC(NULL);

	bmp = CreateCompatibleBitmap( getTargetDC(), drawableBoard.getBoard()->getWidth()*drawableBoard.getTileWidth() + 2, drawableBoard.getBoard()->getHeight() * drawableBoard.getTileHeight() + 2);
	bmpold = (HBITMAP)SelectObject(dcBufferTarget, bmp);
}


void ConsoleBoardRenderer::renderEnd()
{
	BitBlt(dcTarget, drawableBoard.getX(), drawableBoard.getY(), drawableBoard.getBoard()->getWidth()*drawableBoard.getTileWidth() + 2, drawableBoard.getBoard()->getHeight() * drawableBoard.getTileHeight() + 2, dcBufferTarget, 0, 0, SRCCOPY);

	BoardRenderer::renderEnd();

	SelectObject(dcBufferTarget, bmpold);
	DeleteObject(bmp);
	DeleteObject(dcBufferTarget);
}



