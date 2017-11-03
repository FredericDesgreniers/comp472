#pragma once

#include <vector>
#include "vec2.h"
#include <experimental/optional>

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
	std::experimental::optional<std::vector<vec2>> killList;
	bool valid;

public:
	MoveResult(bool valid, std::vector<vec2> killList): valid(valid), killList(killList)
	{

	}

	std::experimental::optional<std::vector<vec2>> getKillList() const
	{
		return killList;
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
	TileType playerType;

public:
	GameMemory();

	bool isPositionOnBoard(vec2 position);
	TileType getTileAt(vec2 position);
	void setTileAt(vec2 position, TileType type);

	TileType* getTileArray()
	{
		return (TileType *)tiles;
	}

	void start();

	void doAiMove();

	void generateTile(vec2 vec2);

	MoveResult doMove(vec2 origin, vec2 destination);

	bool isValidMove(vec2 origin, vec2 destination);

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

	void setPlayerType(TileType playerType)
	{
		this->playerType = playerType;
	}

	TileType getPlayerType()
	{
		return playerType;
	}

	std::vector<vec2>& getCurrentTurnTokenList();
};




