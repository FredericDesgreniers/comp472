#pragma once
#include "BoardRenderer.h"
#include <windows.h>

class WindowsBoardRenderer: public BoardRenderer
{
	HWND handleTarget;
	HDC dcTarget;
	HDC dcBufferTarget;
	HFONT font;
	HBITMAP bmp;
	HBITMAP bmpold;

	char* getRenderCharAndSetColor(Tile *tile);

protected:
	void drawTile(Tile *tile, int x, int y) override;

	void drawBackground() override;

	void renderStart() override;
	void renderEnd() override;

public:
	WindowsBoardRenderer(DrawableBoard drawableBoard, HWND handleTarget);
	Tile* getTileAtDisplayCoordinates(int x, int y) override;

	int getTilePosXFromDisplayCoordinates(int x) override ;

	int getTilePosYFromDisplayCoordinates(int y)override ;

	HDC getTargetDC(){return dcTarget;};
};




