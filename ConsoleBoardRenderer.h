#pragma once
#include "BoardRenderer.h"
#include <Windows.h>
#include <cwchar>

class ConsoleBoardRenderer: public BoardRenderer
{
	HDC dcTarget;
	HFONT font;

protected:
	void drawTile(Tile *tile, int x, int y, int offSetX, int offSetY) override;

	void drawBackground(int offSetX, int offsetY) override;

public:
	ConsoleBoardRenderer(Board board, HDC dcTarget);
};




