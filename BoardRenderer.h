#pragma once
#include "Board.h"
#include <Windows.h>
#include <cwchar>

class BoardRenderer
{
private:
	Board board;
	HDC dcTarget;

	int tileWidth = 20;
	int tileHeight = 20;

	void drawTile(Tile* tile, int x, int y, int offSetX, int offSetY);
public:
	BoardRenderer(Board board, HDC dcTarget);

	void render(int offSetX, int offSetY);
};




