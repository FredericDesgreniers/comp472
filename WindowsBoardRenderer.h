#pragma once
#include "BoardRenderer.h"
#include "DoubleBuffer.h"
#include <windows.h>
#include <memory>

class WindowsBoardRenderer: public BoardRenderer
{
	DoubleBuffer *buffer;

	HFONT font;

	char* getRenderCharAndSetColor(const TileType type);

	COLORREF getTileBackgroundColor(const vec2 position, bool isHovering);

	const vec2 getCursorPosition();

	bool isCursorHovering(const vec2 tilePosition);

	void drawTileBackground(const TileType tile,vec2 tileBoardPosition, const vec2 tilePositionTopLeft);

	void drawTileCharacter(const TileType tile, const vec2 tilePositionTopLeft);

protected:
	void drawTile(vec2 position, TileType  type) override;

	void drawBackground() override;

	void renderStart() override;
	void renderEnd() override;

public:
	WindowsBoardRenderer(TileType* tilesToDraw, vec2 dimension, DoubleBuffer *buffer);

	const vec2 getTilePositionFromDisplayPosition(const vec2 position) override;
};




