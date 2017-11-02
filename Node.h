#pragma once


#include "GameMemory.h"

class Node
{
	int heuristic;

	GameMemory memory;

	void calculateHeuristic();
	void findNextMoves();
	void tryMove(vec2 positon, vec2 direction);

	int depth;

	std::vector<Node> children;

public:
	Node(GameMemory memory, int depth);

	int getHeuristic()
	{

	}

};


