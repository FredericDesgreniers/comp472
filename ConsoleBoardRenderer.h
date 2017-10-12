#pragma once
#include "BoardRenderer.h"
#include <Windows.h>
#include <cwchar>

class ConsoleBoardRenderer: public BoardRenderer
{
	HDC dcTarget;
protected:
	void drawTile(Tile *tile, int x, int y, int offSetX, int offSetY) override;
public:
	ConsoleBoardRenderer(Board board, HDC dcTarget);
};




