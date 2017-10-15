
#include <iostream>
#include "WindowsBoardRenderer.h"

WindowsBoardRenderer::WindowsBoardRenderer(DrawableBoard drawableBoard, HWND handleTarget):BoardRenderer(drawableBoard), handleTarget(handleTarget)
{
	dcTarget = GetDC(handleTarget);

	font = CreateFont(30, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

}

void WindowsBoardRenderer::drawTile(Tile *tile, const vec2 position)
{
	SelectObject( dcBufferTarget, font );

	const vec2 tileDimension = drawableBoard.getTileDimension();

	//TODO remove magic numbers
	// +1 is because of the red border
	const vec2 tilePosition = (position * tileDimension) + 1;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(handleTarget, &pt);

	const vec2 boardPosition = drawableBoard.getPosition();
	const vec2 cursorPoint = {pt.x, pt.y};

	const vec2 relativeMousePosition = cursorPoint - boardPosition;

	const vec2 tileTopLeft = tilePosition;
	const vec2 tileBottomRight = tilePosition + tileDimension;

	bool isHovering = false;
	if(relativeMousePosition.x > tileTopLeft.x &&
			relativeMousePosition.x < tileBottomRight.x
			&& relativeMousePosition.y > tileTopLeft.y &&
			relativeMousePosition.y < tileBottomRight.y)
	{
		isHovering = true;
	}



	HBRUSH brush;
	if(isHovering || tile->getIsSelected())
	{
		brush = CreateSolidBrush(RGB(100,100,100));

	}
	else
	if(tile->getIsBlack())
	{

		brush = CreateSolidBrush(RGB(0,0,0));

	}
	else
	{
		brush = CreateSolidBrush(RGB(150,150,150));
	}

	SelectObject(dcBufferTarget, brush);

	::Rectangle(dcBufferTarget, tilePosition.x, tilePosition.y, tilePosition.x + tileDimension.width, tilePosition.y + tileDimension.height);

	DeleteObject(brush);


	RECT tileDrawRect = {tilePosition.x, tilePosition.y, tilePosition.x + tileDimension.width, tilePosition.y +
			tileDimension.height};


	SetBkMode(dcBufferTarget, TRANSPARENT);

	char* tileChar = getRenderCharAndSetColor(tile);

	DrawText(dcBufferTarget, tileChar, 1, &tileDrawRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
	const vec2 boardDimension = drawableBoard.getBoard()->getDimension();

	vec2 start = {0, 0};
	vec2 end = start + (boardDimension * drawableBoard.getTileDimension()) + 2;

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));

	SelectObject(dcBufferTarget, pen);

	MoveToEx(dcBufferTarget, start.x, start.y, (LPPOINT)NULL);
	LineTo(dcBufferTarget, start.x, end.y);
	LineTo(dcBufferTarget, end.x, end.y);
	LineTo(dcBufferTarget, end.x, start.y);
	LineTo(dcBufferTarget, start.x, start.y);
	DeleteObject(pen);
}

Tile *WindowsBoardRenderer::getTileAtDisplayCoordinates(const vec2 position)
{
	vec2 tilePosition = getTilePositionFromDisplayPosition(position);

	if(tilePosition.x >= 0 && tilePosition.y >= 0)
	{
		return drawableBoard.getBoard()->getTileAt(tilePosition);
	}

	return nullptr;
}


const vec2 WindowsBoardRenderer::getTilePositionFromDisplayPosition(const vec2 position)
{
	vec2 relativePosition = position - drawableBoard.getPosition();

	const vec2 boardPixelDimension = drawableBoard.getPixelDimension();

	if(relativePosition.x > 0 && relativePosition.x < boardPixelDimension.width
		&& relativePosition.y > 0 && relativePosition.y < boardPixelDimension.height)
	{
		vec2 tilePosition = relativePosition / drawableBoard.getTileDimension();

		return tilePosition;
	}

	return {-1, -1};
}

void WindowsBoardRenderer::renderStart()
{
	dcBufferTarget = CreateCompatibleDC(NULL);

	vec2 boardPixelDimension = drawableBoard.getPixelDimension();

	bmp = CreateCompatibleBitmap( getTargetDC(), boardPixelDimension.width + 2, boardPixelDimension.height + 2);
	bmpold = (HBITMAP)SelectObject(dcBufferTarget, bmp);
}


void WindowsBoardRenderer::renderEnd()
{
	vec2 boardPosition = drawableBoard.getPosition();
	vec2 boardPixelDimension = drawableBoard.getPixelDimension();

	BitBlt(dcTarget, boardPosition.x, boardPosition.y, boardPixelDimension.width + 2, boardPixelDimension.height + 2,
	       dcBufferTarget, 0, 0, SRCCOPY);

	BoardRenderer::renderEnd();

	SelectObject(dcBufferTarget, bmpold);
	DeleteObject(bmp);
	DeleteObject(dcBufferTarget);
}



