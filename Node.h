#pragma once


#include "GameMemory.h"
#include <memory>

#define TRACK

struct MoveInfo
{
	vec2 source;
	vec2 destination;

	MoveInfo(vec2 source, vec2 destination):source(source), destination(destination)
	{

	}
};

struct DepthInfo {
	unsigned int depth : 3;
	unsigned int maxDepth : 3;
};

class Node
{
	int heuristic;

	bool isMax;

	GameMemory memory;

	MoveInfo moveInfo;
	MoveInfo bestMove = {0, 0};
	
	

	void calculateHeuristic();
	void  findBestNode(int currentMin, int currentMax);
	
	bool evaluateForBestNode(const std::shared_ptr<Node>&, std::shared_ptr<Node>&, int &currentValue, int &currentMin, int &currentMax);

	DepthInfo depthInfo;

	int simpleHeuristic;

public:
	static unsigned int totalEvaluates;
	static unsigned int totalPrunning;
	static unsigned int totalNodes;

	Node(GameMemory& memory, MoveInfo moveInfo, bool isMax, DepthInfo depthInfo);

	Node(GameMemory memory, unsigned int maxDepth);

	int getHeuristic()
	{
		return heuristic;
	}

	TileType getOpposite(TileType &type)
	{
		if (type == INVALID) return INVALID;
		if (type == RED) return RED;
		if (type == GREEN) return GREEN;

		return EMPTY;
	}

	int evaluateHeuristic(const vec2 &position, const vec2 &direction);

	void evaluate(int currentMin, int currentMax);

	MoveInfo getBestMove()
	{
		return bestMove;
	}

	MoveInfo getMove()
	{
		return moveInfo;
	}

};


