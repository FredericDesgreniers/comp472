#pragma once
#include "BoardRenderer.h"
#include <Windows.h>
#include <cwchar>

class ConsoleBoardRenderer: public BoardRenderer
{
	HWND handleTarget;
	HDC dcTarget;
	HFONT font;

	bool isTileHovered(int x, int y);

protected:
	void drawTile(Tile *tile, int x, int y) override;

	void drawBackground() override;

public:
	ConsoleBoardRenderer(DrawableBoard drawableBoard, HWND handleTarget);
};




