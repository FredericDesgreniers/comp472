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

class MoveResult
{
	bool valid;

public:
	MoveResult(bool valid): valid(valid)
	{

	}



	bool isValid() const
	{
		return valid;
	}
};

class GameMemory
{
	TileType tiles[BOARD_HEIGHT][BOARD_WIDTH];
	TileType currentTurn = GREEN;
	std::vector<vec2> greenPositions;
	std::vector<vec2> redPositions;
	
	unsigned int turnsWithoutAttack = 0;

public:
	GameMemory();

	static TileType playerType;

	bool isPositionOnBoard(vec2 position);
	TileType getTileAt(const vec2 &position);
	void setTileAt(vec2 position, TileType type);

	TileType* getTileArray()
	{
		return (TileType *)tiles;
	}

	void start();

	void doAiMove();

	void generateTile(vec2 vec2);

	MoveResult doMove(vec2 origin, vec2 destination);

	MoveResult doMoveUnsafe(const vec2 &origin, const vec2 &destination);

	bool isValidMove(vec2 origin, vec2 destination);

	void getKillsInDirection(const vec2 origin, const vec2 direction, std::vector<vec2> &killList);

	std::vector<vec2> getKillsInDirection(const vec2 origin, const vec2 direction);

	void nextTurn();

	TileType getCurrentTurn()
	{
		return currentTurn;
	}

	const std::vector<vec2> &getGreenPositions()
	{
		return greenPositions;
	}

	const std::vector<vec2> &getRedPositions()
	{
		return redPositions;
	}

	TileType getPlayerType()
	{
		return playerType;
	}

	std::vector<vec2>& getCurrentTurnTokenList();
};




