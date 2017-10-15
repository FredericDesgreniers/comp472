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

	bool isCursorHovering(const vec2 tilePosition, const vec2 cursorPosition);

protected:
	void drawTile(Tile *tile, const vec2 position) override;

	void drawBackground() override;

	void renderStart() override;
	void renderEnd() override;

public:
	WindowsBoardRenderer(DrawableBoard *drawableBoard, HWND handleTarget);
	Tile* getTileAtDisplayCoordinates(const vec2 position) override;

	const vec2 getTilePositionFromDisplayPosition(const vec2 position) override;

	HDC getTargetDC(){return dcTarget;};
};




