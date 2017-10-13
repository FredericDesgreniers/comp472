
#include "DrawableBoard.h"

DrawableBoard::DrawableBoard(Board *board, int x, int y): board(board), x(x), y(y)
{

}

int DrawableBoard::getX() const
{
	return x;
}

void DrawableBoard::setX(int x)
{
	DrawableBoard::x = x;
}

int DrawableBoard::getY() const
{
	return y;
}

void DrawableBoard::setY(int y)
{
	DrawableBoard::y = y;
}

Board *DrawableBoard::getBoard()
{
	return board;
}

int DrawableBoard::getTileWidth() const
{
	return tileWidth;
}

void DrawableBoard::setTileWidth(int tileWidth)
{
	DrawableBoard::tileWidth = tileWidth;
}

int DrawableBoard::getTileHeight() const
{
	return tileHeight;
}

void DrawableBoard::setTileHeight(int tileHeight)
{
	DrawableBoard::tileHeight = tileHeight;
}

int DrawableBoard::getPixelPositionX()
{
	return x * tileWidth;
}

int DrawableBoard::getPixelPositionY()
{
	return y * tileWidth;
}

int DrawableBoard::getPixelWidth()
{
	return board->getWidth() * tileWidth;
}

int DrawableBoard::getPixelHeight()
{
	return board->getHeight() * tileHeight;
}


