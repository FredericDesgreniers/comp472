#pragma once


#include "GameMemory.h"

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

	GameMemory memory;

	MoveInfo moveInfo;
	MoveInfo bestMove = {0, 0};

	void calculateHeuristic();
	void findNextMoves();
	void tryMove(vec2 positon, vec2 direction);

	int depth;

	std::vector<Node> children;

public:
	Node(Node *parent, GameMemory memory, MoveInfo moveInfo, bool isMax, int depth);

	Node(GameMemory memory);

	int getHeuristic()
	{
		return heuristic;
	}

	Node* getBestNode();

	MoveInfo getBestMove()
	{
		return bestMove;
	}

};


