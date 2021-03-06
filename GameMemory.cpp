
#include <cstring>
#include <algorithm>
#include <iostream>
#include "GameMemory.h"
#include "Node.h"
#include <windows.h>
#include <profileapi.h>

TileType GameMemory::playerType = INVALID;

GameMemory::GameMemory()
{
	memset(tiles, vec2{9, 5}.getArea(), 1);

	for(int yIndex = 0; yIndex < 5; yIndex++)
	{
		for(int xIndex = 0; xIndex < 9; xIndex++)
		{
			generateTile(vec2{xIndex, yIndex});
		}
	}

	std::cout << "Currently "<<redPositions.size() << " red and " << greenPositions.size() << " green "<< std::endl;
}



TileType GameMemory::getTileAt(const vec2 &position)
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
		switch(tiles[position.y][position.x])
		{
			case GREEN:
			{
				greenPositions.erase(std::remove(greenPositions.begin(), greenPositions.end(),  position));
			}break;
			case RED:
			{
				redPositions.erase(std::remove(redPositions.begin(), redPositions.end(), position));
			}break;
		}

		switch(type)
		{
			case GREEN:
			{
				greenPositions.push_back(position);
			}break;
			case RED:
			{
				redPositions.push_back(position);
			}break;
		}

		tiles[position.y][position.x] = type;
	}
}

bool GameMemory::isPositionOnBoard(vec2 position)
{
	return position.x >= 0 && position.x < BOARD_WIDTH &&
	       position.y >= 0 && position.y < BOARD_HEIGHT;
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
	if (!isValidMove(origin, destination))
	{
		return { false};
	}

	return doMoveUnsafe(origin, destination);
}

MoveResult GameMemory::doMoveUnsafe(const vec2 &origin, const vec2 &destination)
{

	vec2 direction = destination - origin;

	TileType originTile = getTileAt(origin);

	std::vector<vec2> killList;
	killList.reserve(5);
	getKillsInDirection(origin + direction, direction, killList);


	if (killList.size() == 0)
	{
		killList.clear();
		getKillsInDirection(origin, -direction, killList);
	}

	if (killList.size() > 0)
	{
		turnsWithoutAttack = 0;
	}
	else
	{
		turnsWithoutAttack++;
	}

	for (auto &killPosition : killList)
	{
		setTileAt(killPosition, EMPTY);
	}

	setTileAt(destination, originTile);
	setTileAt(origin, EMPTY);

	return { true };
}

bool GameMemory::isValidMove(vec2 origin, vec2 destination)
{
	vec2 direction = destination - origin;
	vec2 absDirection = direction.getAbs();

	TileType originTile = getTileAt(origin);

	if(originTile != currentTurn || originTile == INVALID || originTile ==EMPTY)
	{
		return false;
	}

	bool diagonal = absDirection.x == 1 && absDirection.y == 1;

	if(diagonal && !isBlackReferenceBoard[origin.y][origin.x])
	{
		return false;
	}

	if(getTileAt(destination) != EMPTY)
	{
		return false;
	}

	return true;
}
void GameMemory::getKillsInDirection(const vec2 origin, const vec2 direction, std::vector<vec2> &killList)
{
	const auto nextTileInDirection = origin + direction;
	TileType tileType = getTileAt(nextTileInDirection);

	if (tileType == currentTurn || tileType == EMPTY || tileType == INVALID)
	{
		return;
	}

	killList.push_back(nextTileInDirection);

	getKillsInDirection(nextTileInDirection, direction, killList);
}


std::vector<vec2> GameMemory::getKillsInDirection(const vec2 origin, const vec2 direction)
{
	std::vector<vec2> killList;
	getKillsInDirection(origin, direction, killList);
	return killList;
}

void GameMemory::nextTurn()
{
	std::cout << "Turns Without Attack: " << turnsWithoutAttack << std::endl;
	if(redPositions.size() == 0)
	{
		std::cout << "GREEN WON" << std::endl;
		return;
	}
	if(greenPositions.size() == 0)
	{
		std::cout << "RED WON" << std::endl;
		return;
	}
	if (turnsWithoutAttack==10)
	{
		std::cout << "GAME ENDS IN DRAW" << std::endl;
		return;
	}

	currentTurn = currentTurn == GREEN? RED:GREEN;
	std::cout << "Current turn is " << (currentTurn == GREEN?"GREEN":"RED") << std::endl;

	if(playerType != currentTurn)
	{
		doAiMove();
	}

}

void GameMemory::start()
{
	if(playerType != currentTurn)
	{
		doAiMove();
	}
}

void GameMemory::doAiMove()
{
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	double elapsedTime;
	// get ticks per second
	QueryPerformanceFrequency(&frequency);

	// start timer
	QueryPerformanceCounter(&t1);


	Node::totalEvaluates = 0;
	Node::totalPrunning = 0;
	Node::totalNodes = 0;

	Node node(*this, 5);
	node.evaluate(INT_MIN, INT_MAX);
	// stop timer
	QueryPerformanceCounter(&t2);

	// compute and print the elapsed time in millisec
	elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
	std::cout << "Time for move: " << elapsedTime / 1000 << "s" << std::endl;


	std::cout << "Nodes: " << Node::totalNodes << std::endl;
	std::cout << "Evaluates: " << Node::totalEvaluates << std::endl;
	std::cout << "Prunnings: " << Node::totalPrunning << std::endl;
	

	auto bestMove = node.getBestMove();
	doMove(bestMove.source, bestMove.destination);
	std::cout << "Moving from " << bestMove.source.getBoardCoordinates() << ", " << bestMove.destination.getBoardCoordinates() << std::endl;
	std::cout << "h: " << node.getHeuristic() << std::endl;

	

	nextTurn();
}

std::vector<vec2> &GameMemory::getCurrentTurnTokenList()
{
	switch(getCurrentTurn())
	{
		case RED:
		{
			return redPositions;
		}

		case GREEN:
		{
			return greenPositions;
		}

		default:
			{
			return greenPositions;
			}
	}
}

