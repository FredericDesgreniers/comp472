
#include <iostream>
#include "WindowsBoardRenderer.h"

WindowsBoardRenderer::WindowsBoardRenderer(DrawableBoard *drawableBoard, HWND handleTarget):BoardRenderer
		                                                                                            (drawableBoard), handleTarget(handleTarget)
{
	dcTarget = GetDC(handleTarget);

	font = CreateFont(30, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

}

void WindowsBoardRenderer::renderStart()
{
	dcBufferTarget = CreateCompatibleDC(NULL);

	const vec2 boardDimensionIncludingBorder = drawableBoard->getPixelDimension() + (getDrawableBoard()->getBorderWeight() * 2);

	bmp = CreateCompatibleBitmap( getTargetDC(), boardDimensionIncludingBorder.width,
	                              boardDimensionIncludingBorder.height);
	bmpold = (HBITMAP)SelectObject(dcBufferTarget, bmp);
}

void WindowsBoardRenderer::drawTile(Tile *tile, const vec2 position)
{
	SelectObject( dcBufferTarget, font );

	const vec2 tilePosition = (position * drawableBoard->getTileDimension()) + drawableBoard->getBorderWeight();

	drawTileBackground(tile, tilePosition);

	drawTileCharacter(tile, tilePosition);
}


void WindowsBoardRenderer::drawTileBackground(const Tile *tile, const vec2 tilePositionTopLeft)
{
	const COLORREF tileBackgroundColor = getTileBackgroundColor(tile, isCursorHovering(tilePositionTopLeft));

	const HBRUSH tileBackgroundBrush = CreateSolidBrush(tileBackgroundColor);
	SelectObject(dcBufferTarget, tileBackgroundBrush);

	const vec2 tilePositionBottomRight = tilePositionTopLeft + drawableBoard->getTileDimension();

	::Rectangle(dcBufferTarget, tilePositionTopLeft.x, tilePositionTopLeft.y, tilePositionBottomRight.x,
	            tilePositionBottomRight.y);

	DeleteObject(tileBackgroundBrush);
}

bool WindowsBoardRenderer::isCursorHovering(const vec2 tilePosition)
{
	const vec2 relativeMousePosition = getCursorPosition() - drawableBoard->getPosition();

	const vec2 &tileTopLeft = tilePosition;
	const vec2 tileBottomRight = tilePosition + drawableBoard->getTileDimension();

	bool isHovering = false;
	if(relativeMousePosition.x > tileTopLeft.x &&
	   relativeMousePosition.x < tileBottomRight.x
	   && relativeMousePosition.y > tileTopLeft.y &&
	   relativeMousePosition.y < tileBottomRight.y)
	{
		isHovering = true;
	}
	return isHovering;

}

const vec2 WindowsBoardRenderer::getCursorPosition()
{
	POINT cursorPosition;
	GetCursorPos(&cursorPosition);
	ScreenToClient(handleTarget, &cursorPosition);

	return vec2{cursorPosition.x, cursorPosition.y};
}

COLORREF WindowsBoardRenderer::getTileBackgroundColor(const Tile *tile, bool isHovering)
{
	if(isHovering || tile->getIsSelected())
	{
		return RGB(100,100,100);
	}
	else
	if(tile->getIsBlack())
	{
		return RGB(0,0,0);
	}
	else
	{
		return RGB(150,150,150);
	}

}

void WindowsBoardRenderer::drawTileCharacter(const Tile *tile, const vec2 tilePositionTopLeft)
{
	const vec2 tilePositionBottomRight = tilePositionTopLeft + drawableBoard->getTileDimension();

	RECT tileCharRect = {tilePositionTopLeft.x, tilePositionTopLeft.y, tilePositionBottomRight.x,
	                     tilePositionBottomRight.y};

	const char* tileChar = getRenderCharAndSetColor(tile);

	SetBkMode(dcBufferTarget, TRANSPARENT);
	DrawText(dcBufferTarget, tileChar, 1, &tileCharRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	SetBkMode(dcBufferTarget, OPAQUE);
}

char *WindowsBoardRenderer::getRenderCharAndSetColor(const Tile *tile)
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
	const vec2 boardDimension = drawableBoard->getBoard()->getDimension();

	vec2 start = {0, 0};
	vec2 end = start + (boardDimension * drawableBoard->getTileDimension()) + 2;

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
		return drawableBoard->getBoard()->getTileAt(tilePosition);
	}

	return nullptr;
}


const vec2 WindowsBoardRenderer::getTilePositionFromDisplayPosition(const vec2 position)
{
	vec2 relativePosition = position - drawableBoard->getPosition();

	const vec2 boardPixelDimension = drawableBoard->getPixelDimension();

	if(relativePosition.x > 0 && relativePosition.x < boardPixelDimension.width
		&& relativePosition.y > 0 && relativePosition.y < boardPixelDimension.height)
	{
		vec2 tilePosition = relativePosition / drawableBoard->getTileDimension();

		return tilePosition;
	}

	return {-1, -1};
}

void WindowsBoardRenderer::renderEnd()
{
	vec2 boardPosition = drawableBoard->getPosition();
	vec2 boardPixelDimension = drawableBoard->getPixelDimension();

	BitBlt(dcTarget, boardPosition.x, boardPosition.y, boardPixelDimension.width + 2, boardPixelDimension.height + 2,
	       dcBufferTarget, 0, 0, SRCCOPY);

	BoardRenderer::renderEnd();

	SelectObject(dcBufferTarget, bmpold);
	DeleteObject(bmp);
	DeleteObject(dcBufferTarget);
}




