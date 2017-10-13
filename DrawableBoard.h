

#pragma once


#include "Board.h"

class DrawableBoard
{
private:
	Board *board;

	int x;
	int y;

	int tileWidth = 30;
	int tileHeight = 30;

public:
	DrawableBoard(Board *board, int x, int y);

	int getX() const;

	void setX(int x);

	int getY() const;

	void setY(int y);

	Board *getBoard();

	int getTileWidth() const;

	void setTileWidth(int tileWidth);

	int getTileHeight() const;

	void setTileHeight(int tileHeight);

};




