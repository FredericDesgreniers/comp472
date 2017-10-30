#pragma once

#include <vector>
#include "vec2.h"

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 5

enum TileType
{
	INVALID, EMPTY, GREEN, RED
};

static bool isBlackReferenceBoard[5][9] =
		{
			1, 0, 1, 0, 1,
			0, 1, 0, 1, 0,
			1, 0, 1, 0, 1,
			0, 1, 0, 1, 0,
			1, 0, 1, 0, 1,
			0, 1, 0, 1, 0,
			1, 0, 1, 0, 1,
			0, 1, 0, 1, 0,
			1, 0, 1, 0, 1,
		};

class GameMemory
{
	TileType tiles[BOARD_HEIGHT][BOARD_WIDTH];
	TileType currentTurn = GREEN;

public:
	GameMemory();

	bool isPositionOnBoard(vec2 position);
	TileType getTileAt(vec2 position);
	void setTileAt(vec2 position, TileType type);

	TileType* getTileArray()
	{
		return (TileType *)tiles;
	}

	void generateTile(vec2 vec2);

	std::vector<vec2> doMoveAndReturnKillList(vec2 origin, vec2 destination);
};




