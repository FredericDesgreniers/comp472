
#include <iostream>
#include "WindowsBoardRenderer.h"

WindowsBoardRenderer::WindowsBoardRenderer(DrawableBoard drawableBoard, HWND handleTarget):BoardRenderer(drawableBoard), handleTarget(handleTarget)
{
	dcTarget = GetDC(handleTarget);

	font = CreateFont(30, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

}

void WindowsBoardRenderer::drawTile(Tile *tile, int x, int y)
{
	SelectObject( dcBufferTarget, font );

	//TODO remove magic numbers
	// +1 is because of the red border
	int tilePosX = x * drawableBoard.getTileWidth() + 1;
	int tilePosY = y * drawableBoard.getTileHeight() + 1;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(handleTarget, &pt);

	int relativeMousePosX = pt.x - drawableBoard.getX();
	int relativeMousePosY = pt.y - drawableBoard.getY();

	bool isHovering = false;
	if(relativeMousePosX > tilePosX &&
			relativeMousePosX < tilePosX + drawableBoard.getTileWidth()
			&& relativeMousePosY > tilePosY &&
			relativeMousePosY < tilePosY + drawableBoard.getTileHeight())
	{
		isHovering = true;
	}

	bool xMod2 = x % 2;
	bool yMod2 = y % 2;

	HBRUSH brush;
	if(isHovering || tile->getIsSelected())
	{
		brush = CreateSolidBrush(RGB(150,150,150));

	}
	else
	if((xMod2 || yMod2) && !(xMod2 && yMod2))
	{

		brush = CreateSolidBrush(RGB(50,50,50));

	}
	else
	{
		brush = CreateSolidBrush(RGB(150,150,150));
	}

	SelectObject(dcBufferTarget, brush);

	::Rectangle(dcBufferTarget, tilePosX, tilePosY, tilePosX + drawableBoard.getTileWidth(), tilePosY + drawableBoard.getTileHeight());

	DeleteObject(brush);


	RECT tileDimension = {tilePosX, tilePosY, tilePosX + drawableBoard.getTileWidth(), tilePosY + drawableBoard.getTileHeight()};


	SetBkMode(dcBufferTarget, TRANSPARENT);

	char* tileChar = getRenderCharAndSetColor(tile);

	DrawText(dcBufferTarget, tileChar, 1, &tileDimension, DT_CENTER);
}

char *WindowsBoardRenderer::getRenderCharAndSetColor(Tile *tile)
{
	switch(tile->getType())
	{
		case EMPTY:
		{
			return " ";
		};
		case RED:
		{
			SetTextColor(dcBufferTarget, RGB(255, 0, 0));
			return "R";
		};
		case GREEN:
		{
			SetTextColor(dcBufferTarget, RGB(0, 255, 0));
			return "G";
		};

		default:
		{
			return " ";
		}
	}
}

void WindowsBoardRenderer::drawBackground()
{
	int startX = 0;
	int startY = 0;
	int endX = startX + drawableBoard.getBoard()->getWidth() * drawableBoard.getTileWidth() + 2;
	int endY = startY + drawableBoard.getBoard()->getHeight() * drawableBoard.getTileHeight() + 2;

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));

	SelectObject(dcBufferTarget, pen);

	MoveToEx(dcBufferTarget, startX, startY, (LPPOINT)NULL);
	LineTo(dcBufferTarget, startX, endY);
	LineTo(dcBufferTarget, endX, endY);
	LineTo(dcBufferTarget, endX, startY);
	LineTo(dcBufferTarget, startX, startY);
	DeleteObject(pen);
}

Tile *WindowsBoardRenderer::getTileAtDisplayCoordinates(int x, int y)
{
	int tilePosX = getTilePosXFromDisplayCoordinates(x);
	int tilePosY = getTilePosYFromDisplayCoordinates(y);

	if(tilePosX >= 0 && tilePosY >= 0)
	{
		return drawableBoard.getBoard()->getTileAt(tilePosX, tilePosY);
	}

	return nullptr;
}

int WindowsBoardRenderer::getTilePosXFromDisplayCoordinates(int x)
{
	int relativeX = x - drawableBoard.getX();

	if(relativeX > 0 && relativeX < drawableBoard.getPixelWidth())
	{
		int tilePosX = relativeX / drawableBoard.getTileWidth();

		return tilePosX;
	}

	return -1;
}

int WindowsBoardRenderer::getTilePosYFromDisplayCoordinates(int y)
{
	int relativeY = y - drawableBoard.getY();

	if(relativeY > 0 && relativeY < drawableBoard.getPixelHeight())
	{
		int tilePosY = relativeY / drawableBoard.getTileHeight();

		return tilePosY;
	}

	return -1;
}

void WindowsBoardRenderer::renderStart()
{
	dcBufferTarget = CreateCompatibleDC(NULL);

	bmp = CreateCompatibleBitmap( getTargetDC(), drawableBoard.getPixelWidth() + 2, drawableBoard.getPixelHeight() + 2);
	bmpold = (HBITMAP)SelectObject(dcBufferTarget, bmp);
}


void WindowsBoardRenderer::renderEnd()
{
	BitBlt(dcTarget, drawableBoard.getX(), drawableBoard.getY(), drawableBoard.getPixelWidth() + 2, drawableBoard.getPixelHeight() + 2, dcBufferTarget, 0, 0, SRCCOPY);

	BoardRenderer::renderEnd();

	SelectObject(dcBufferTarget, bmpold);
	DeleteObject(bmp);
	DeleteObject(dcBufferTarget);
}





