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

	char* getRenderCharAndSetColor(const Tile *tile);

	COLORREF getTileBackgroundColor(const Tile *tile, bool isHovering);

	const vec2 getCursorPosition();

	bool isCursorHovering(const vec2 tilePosition);

	void drawTileBackground(const Tile* tile, const vec2 tilePositionTopLeft);

	void drawTileCharacter(const Tile* tile, const vec2 tilePositionTopLeft);

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




