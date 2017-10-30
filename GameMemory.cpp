
#include <cstring>
#include <algorithm>
#include <iostream>
#include "GameMemory.h"

bool GameMemory::isPositionOnBoard(vec2 position)
{
	return position.x >= 0 && position.x < BOARD_WIDTH &&
			position.y >= 0 && position.y < BOARD_HEIGHT;
}

TileType GameMemory::getTileAt(vec2 position)
{
	if(!isPositionOnBoard(position))
	{
		return INVALID;
	}
	return tiles[position.y][position.x];
}

void GameMemory::setTileAt(vec2 position, TileType type)
{
	if(isPositionOnBoard(position))
	{
		tiles[position.y][position.x] = type;
	}
}

GameMemory::GameMemory()
{
	memset(tiles, getArea({9, 5}), 1);

	for(int yIndex = 0; yIndex < 5; yIndex++)
	{
		for(int xIndex = 0; xIndex < 9; xIndex++)
		{
			generateTile(vec2{xIndex, yIndex});
		}
	}
}

void GameMemory::generateTile(vec2 position)
{
	if(position.y < 2)
	{
		setTileAt(position, RED);
	}
	else if(position.y > 2)
	{
		setTileAt(position, GREEN);
	}
	else
	{
		if(position.x <4)
		{
			setTileAt(position, GREEN);
		}
		else if(position.x > 4)
		{
			setTileAt(position, RED);
		}
		else
		{
			setTileAt(position, EMPTY);
		}

	}
}

MoveResult GameMemory::doMove(vec2 origin, vec2 destination)
{
	vec2 direction = destination - origin;
	vec2 absDirection = direction.getAbs();

	TileType originTile = getTileAt(origin);

	if(originTile != currentTurn)
	{
		return {false, std::vector<vec2>()};
	}

	bool diagonal = absDirection.x == 1 && absDirection.y == 1;

	if(diagonal && !isBlackReferenceBoard[origin.y][origin.x])
	{
		return MoveResult(false, std::vector<vec2>());
	}


	auto killList = getKillsInDirection(origin+direction, direction);
	if(killList.size() > 0)
	{
		//do at 0
	}
	else
	{
		killList = getKillsInDirection(origin, -direction);
	}

	for(auto it = killList.begin(); it != killList.end(); it++)
	{
		setTileAt(*it, EMPTY);
	}

	setTileAt(destination, originTile);
	setTileAt(origin, EMPTY);

	currentTurn = currentTurn == GREEN? RED:GREEN;

	return {true, killList};
}

std::vector<vec2> GameMemory::getKillsInDirection(const vec2 origin, const vec2 direction)
{
	const auto nextTileInDirection = origin + direction;
	TileType tileType = getTileAt(nextTileInDirection);

	if(tileType == currentTurn || tileType == EMPTY || tileType == INVALID)
	{
		return std::vector<vec2>();
	}

	std::vector<vec2> killList;
	killList.push_back(nextTileInDirection);

	std::vector<vec2> nextKills = getKillsInDirection(nextTileInDirection, direction);

	killList.insert(std::end(killList), std::begin(nextKills), std::end(nextKills));

	return killList;
}
