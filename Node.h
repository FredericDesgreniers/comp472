#pragma once


#include "GameMemory.h"
#include <memory>
#define maxDepth 6

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

	Node *parent;

	GameMemory& memory;

	MoveInfo moveInfo;
	MoveInfo bestMove = {0, 0};
	
	std::shared_ptr<Node> bestNode;

	void calculateHeuristic();
	std::shared_ptr<Node>  findBestNode();

	int depth;

	int currentMin, currentMax;

public:
	Node(Node *parent, GameMemory &memory, MoveInfo moveInfo, bool isMax, int depth, int currentMin, int currentMax);

	Node(GameMemory &memory);

	int getHeuristic()
	{
		return heuristic;
	}


	MoveInfo getBestMove()
	{
		return bestMove;
	}

	MoveInfo getMove()
	{
		return moveInfo;
	}

};


