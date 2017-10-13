#pragma once
#include "BoardRenderer.h"
#include <Windows.h>
#include <cwchar>

class ConsoleBoardRenderer: public BoardRenderer
{
	HWND handleTarget;
	HDC dcTarget;
	HDC dcBufferTarget;
	HFONT font;
	HBITMAP bmp;
	HBITMAP bmpold;

	char* getRenderChar(Tile *tile);

protected:
	void drawTile(Tile *tile, int x, int y) override;

	void drawBackground() override;

	void renderStart() override;
	void renderEnd() override;

public:
	ConsoleBoardRenderer(DrawableBoard drawableBoard, HWND handleTarget);
	Tile* getTileAtDisplayCoordinates(int x, int y) override;

	int getTilePosXFromDisplayCoordinates(int x) override ;

	int getTilePosYFromDisplayCoordinates(int y)override ;

	HDC getTargetDC(){return dcTarget;};
};




