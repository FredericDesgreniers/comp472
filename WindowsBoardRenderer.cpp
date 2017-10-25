
#include <iostream>
#include "WindowsBoardRenderer.h"

WindowsBoardRenderer::WindowsBoardRenderer(TileType *tilesToDraw, vec2 dimension, HWND handleTarget):BoardRenderer(tilesToDraw, dimension), handleTarget(handleTarget)
{
	dcTarget = GetDC(handleTarget);

	font = CreateFont(30, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");

}

void WindowsBoardRenderer::renderStart()
{
	dcBufferTarget = CreateCompatibleDC(NULL);

	const vec2 boardDimensionIncludingBorder = pixelDimension;

	bmp = CreateCompatibleBitmap( getTargetDC(), boardDimensionIncludingBorder.width,
	                              boardDimensionIncludingBorder.height);
	bmpold = (HBITMAP)SelectObject(dcBufferTarget, bmp);
}

void WindowsBoardRenderer::drawTile(vec2 position, TileType  tileType)
{
	const vec2 tilePosition = (position * tileDimension);

	drawTileBackground(tileType, position, tilePosition);

	drawTileCharacter(tileType, tilePosition);
}


void WindowsBoardRenderer::drawTileBackground(const TileType tileType, vec2 tileBoardPosition, const vec2 tilePositionTopLeft)
{
	const COLORREF tileBackgroundColor = getTileBackgroundColor(tileBoardPosition, isCursorHovering(tilePositionTopLeft));

	const HBRUSH tileBackgroundBrush = CreateSolidBrush(tileBackgroundColor);
	SelectObject(dcBufferTarget, tileBackgroundBrush);

	const vec2 tilePositionBottomRight = tilePositionTopLeft + tileDimension;

	::Rectangle(dcBufferTarget, tilePositionTopLeft.x, tilePositionTopLeft.y, tilePositionBottomRight.x,
	            tilePositionBottomRight.y);

	DeleteObject(tileBackgroundBrush);
}

bool WindowsBoardRenderer::isCursorHovering(const vec2 tilePosition)
{
	const vec2 relativeMousePosition = getCursorPosition();

	const vec2 &tileTopLeft = tilePosition;
	const vec2 tileBottomRight = tilePosition + tileDimension;

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

COLORREF WindowsBoardRenderer::getTileBackgroundColor(const vec2 position, bool isHovering)
{

	if(isHovering)
	{
		return RGB(100, 100, 100);
	}
	else
	if(isBlackReferenceBoard[position.y][position.x])
	{
		return RGB(0,0,0);
	}
	else
	{
		return RGB(150, 150, 150);
	}

}

void WindowsBoardRenderer::drawTileCharacter(TileType type, const vec2 tilePositionTopLeft)
{
	const vec2 tilePositionBottomRight = tilePositionTopLeft + tileDimension;

	RECT tileCharRect = {tilePositionTopLeft.x, tilePositionTopLeft.y, tilePositionBottomRight.x,
	                     tilePositionBottomRight.y};

	const char* tileChar = getRenderCharAndSetColor(type);

	SelectObject( dcBufferTarget, font );

	SetBkMode(dcBufferTarget, TRANSPARENT);
	DrawText(dcBufferTarget, tileChar, 1, &tileCharRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	SetBkMode(dcBufferTarget, OPAQUE);
}

char *WindowsBoardRenderer::getRenderCharAndSetColor(TileType type)
{
	switch(type)
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
	const vec2 boardDimension = boardDimension;

	vec2 start = {0, 0};
	vec2 end = start + (boardDimension * tileDimension) + 2;

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0,0,0));

	SelectObject(dcBufferTarget, pen);

	MoveToEx(dcBufferTarget, start.x, start.y, (LPPOINT)NULL);
	LineTo(dcBufferTarget, start.x, end.y);
	LineTo(dcBufferTarget, end.x, end.y);
	LineTo(dcBufferTarget, end.x, start.y);
	LineTo(dcBufferTarget, start.x, start.y);
	DeleteObject(pen);
}

const vec2 WindowsBoardRenderer::getTilePositionFromDisplayPosition(const vec2 position)
{
	vec2 relativePosition = position;

	const vec2 boardPixelDimension = pixelDimension;

	if(relativePosition.x > 0 && relativePosition.x < boardPixelDimension.width
		&& relativePosition.y > 0 && relativePosition.y < boardPixelDimension.height)
	{
		vec2 tilePosition = relativePosition / tileDimension;

		return tilePosition;
	}

	return {-1, -1};
}

void WindowsBoardRenderer::renderEnd()
{
	vec2 boardPosition = 0;
	vec2 boardPixelDimension = pixelDimension;

	BitBlt(dcTarget, boardPosition.x, boardPosition.y, boardPixelDimension.width + 2, boardPixelDimension.height + 2,
	       dcBufferTarget, 0, 0, SRCCOPY);

	BoardRenderer::renderEnd();

	SelectObject(dcBufferTarget, bmpold);
	DeleteObject(bmp);
	DeleteObject(dcBufferTarget);
}




