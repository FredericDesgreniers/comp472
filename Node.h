#pragma once


#include "GameMemory.h"
#include <memory>
#define maxDepth 6

#define TRACK

struct MoveInfo
{
	vec2 source;
	vec2 destination;

	MoveInfo(vec2 source, vec2 destination):source(source), destination(destination)
	{

	}
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

	int depth;

	int simpleHeuristic;

public:
	static int totalEvaluates;
	static int totalPrunning;
	static int totalNodes;

	Node(GameMemory& memory, MoveInfo moveInfo, bool isMax, int depth);

	Node(GameMemory memory);

	int getHeuristic()
	{
		return heuristic;
	}

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


