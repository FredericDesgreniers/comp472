
#include <iostream>
#include "WindowsBoardRenderer.h"

WindowsBoardRenderer::WindowsBoardRenderer(TileType *tilesToDraw, vec2 dimension, DoubleBuffer *buffer):BoardRenderer(tilesToDraw, dimension), buffer(buffer)
{
	font = CreateFont(30, 15, 0, 0, 500, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
}

void WindowsBoardRenderer::renderStart()
{
	buffer->allocateBuffer();
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
	SelectObject(buffer->getUsableDC(), tileBackgroundBrush);

	const vec2 tilePositionBottomRight = tilePositionTopLeft + tileDimension;

	::Rectangle(buffer->getUsableDC(), tilePositionTopLeft.x, tilePositionTopLeft.y, tilePositionBottomRight.x,
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
	ScreenToClient(buffer->getHandle(), &cursorPosition);

	return vec2{cursorPosition.x, cursorPosition.y};
}

COLORREF WindowsBoardRenderer::getTileBackgroundColor(const vec2 position, bool isHovering)
{

	if(isHovering || selectedTile == position)
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

	SelectObject( buffer->getUsableDC(), font );

	SetBkMode(buffer->getUsableDC(), TRANSPARENT);
	DrawText(buffer->getUsableDC(), tileChar, 1, &tileCharRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	SetBkMode(buffer->getUsableDC(), OPAQUE);
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
			SetTextColor(buffer->getUsableDC(), RGB(255, 0, 0));
			return "R";
		};
		case GREEN:
		{
			SetTextColor(buffer->getUsableDC(), RGB(0, 255, 0));
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

	SelectObject(buffer->getUsableDC(), pen);

	MoveToEx(buffer->getUsableDC(), start.x, start.y, (LPPOINT)NULL);
	LineTo(buffer->getUsableDC(), start.x, end.y);
	LineTo(buffer->getUsableDC(), end.x, end.y);
	LineTo(buffer->getUsableDC(), end.x, start.y);
	LineTo(buffer->getUsableDC(), start.x, start.y);
	DeleteObject(pen);
	SelectObject( buffer->getUsableDC(), font );
	RECT numberPos =
			{
					0,
			        tileBoardDimension.height*tileDimension.height,
			        tileDimension.width,
			        tileBoardDimension.height*tileDimension.height + tileDimension.height
			};
	SetBkMode(buffer->getUsableDC(), TRANSPARENT);
	for(auto numChar = '1'; numChar <= '9'; numChar ++)
	{
		DrawText(buffer->getUsableDC(), &numChar, 1, &numberPos ,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		numberPos.left += tileDimension.width;
		numberPos.right += tileDimension.width;
	}

	RECT charPos =
			{
					tileBoardDimension.width * tileDimension.width,
					0,
					tileBoardDimension.width * tileDimension.width + tileDimension.width,
					tileDimension.height
			};
	for(auto c = 'A'; c <= 'E'; c++)
	{
		DrawText(buffer->getUsableDC(), &c, 1, &charPos ,DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		charPos.top += tileDimension.height;
		charPos.bottom += tileDimension.height;
	}
	SetBkMode(buffer->getUsableDC(), OPAQUE);

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
	buffer->swapBuffers();

	BoardRenderer::renderEnd();
}




