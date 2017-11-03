//
// Created by frede on 2017-11-02.
//

#include <iostream>
#include "Node.h"

Node::Node(Node *parent, GameMemory &memory, MoveInfo moveInfo, bool isMax, int depth): parent(parent), memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth)
{
	if(depth == maxDepth)
	{
		calculateHeuristic();
	}

	if (depth < maxDepth)
	{
		findNextMoves();
		auto bestNode = getBestNode();
		heuristic = bestNode->heuristic;
		bestMove = bestNode->moveInfo;
	}


}

void Node::calculateHeuristic()
{
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
	auto &tokenList = memory.getCurrentTurnTokenList();

	for(const auto &token : tokenList)
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
	if(memory.isValidMove(position, destination))
	{
		GameMemory newMemory = memory;

		if (newMemory.doMove(position, destination).isValid())
		{
			children.push_back(Node(this, newMemory, {position, destination}, !isMax, depth + 1));
		}
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