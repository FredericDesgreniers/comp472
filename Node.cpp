//
// Created by frede on 2017-11-02.
//

#include <iostream>
#include "Node.h"

Node::Node(Node *parent, GameMemory memory, MoveInfo moveInfo, bool isMax, int depth): parent(parent), memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth)
{

	if(depth == 3)
	{
		calculateHeuristic();
	}

	if (depth < 3)
	{
		findNextMoves();
		auto bestNode = getBestNode();
		heuristic = bestNode->heuristic;
		bestMove = bestNode->moveInfo;
	}


}

void Node::calculateHeuristic()
{
	heuristic =  memory.getGreenPositions().size() - memory.getRedPositions().size();

	return;
	int greenHorizontalScore = 0;
	int greenVerticalScore = 0;

	auto &greenTokens = memory.getGreenPositions();

	for(const auto greenToken : memory.getGreenPositions())
	{
		greenHorizontalScore += greenToken.x * greenTokens.size();
		greenVerticalScore += greenToken.y * greenTokens.size();
	}

	int redHorizontalScore = 0;
	int redVerticalScore = 0;

	auto &redTokens = memory.getRedPositions();

	for(const auto redToken : redTokens)
	{
		redHorizontalScore += redToken.x * redTokens.size();
		redVerticalScore += redToken.y * redTokens.size();
	}

	heuristic = 100 * greenHorizontalScore + 50 * greenVerticalScore - 100 * redHorizontalScore - 50 * redVerticalScore;
}

void Node::findNextMoves()
{
	std::vector<vec2> tokenList;

	switch(memory.getCurrentTurn())
	{
		case RED:
		{
			tokenList = memory.getRedPositions();
		}break;

		case GREEN:
		{
			tokenList = memory.getGreenPositions();
		}break;
	}

	for(const auto token : tokenList)
	{
		for(int dx = -1; dx <= 1; dx++)
		{
			for(int dy = -1; dy <= 1; dy++)
			{
				if(!(dx == 0 && dy == 0))
				{
					tryMove(token, {dx, dy});
				}
			}
		}

	}
}

void Node::tryMove(vec2 position, vec2 direction)
{
	auto destination = position + direction;
	GameMemory newMemory = memory;

	if (newMemory.doMove(position, destination).isValid())
	{
		children.push_back(Node(this, newMemory, {position, destination}, !isMax, depth + 1));
	}

}

Node *Node::getBestNode()
{

	Node *bestChild = nullptr;
	for(auto &child : children)
	{
		if(bestChild == nullptr)
		{
			bestChild = &child;
		}
		else
		{
			if(isMax)
			{
				if (bestChild->getHeuristic() < child.getHeuristic())
				{
					bestChild = &child;
				}
			}
			else
			{
				if (bestChild->getHeuristic() > child.getHeuristic())
				{
					bestChild = &child;
				}
			}
		}
	}

	return bestChild;
}

Node::Node(GameMemory memory):Node(nullptr, memory, {0,0}, memory.getCurrentTurn() == GREEN,0)
{
	std::cout << "Finding for " << (isMax?"MAX":"MIN") << std::endl;
}
