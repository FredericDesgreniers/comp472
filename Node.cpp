//
// Created by frede on 2017-11-02.
//

#include <iostream>
#include "Node.h"
#include <algorithm>

Node::Node(Node *parent, GameMemory &memory, MoveInfo moveInfo, bool isMax, int depth, int currentMin, int currentMax): parent(parent), memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth), currentMin(currentMin), currentMax(currentMax)
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

	for(const auto &greenToken : memory.getGreenPositions())
	{
		greenHorizontalScore += greenToken.x * greenTokens.size();
		greenVerticalScore += greenToken.y * greenTokens.size();
	}

	int redHorizontalScore = 0;
	int redVerticalScore = 0;

	auto &redTokens = memory.getRedPositions();

	for(const auto &redToken : redTokens)
	{
		redHorizontalScore += redToken.x * redTokens.size();
		redVerticalScore += redToken.y * redTokens.size();
	}

	heuristic = greenTokens.size() - redTokens.size();
}

void Node::findNextMoves()
{
	auto &tokenList = memory.getCurrentTurnTokenList();

	int currentValue = isMax ? INT_MIN : INT_MAX;

	for(const auto &token : tokenList)
	{
		for(int dx = -1; dx <= 1; dx++)
		{
			for(int dy = -1; dy <= 1; dy++)
			{
				if(!(dx == 0 && dy == 0))
				{
					auto position = token;
					auto direction = vec2{ dx, dy };

					auto destination = position + direction;
					if (memory.isValidMove(position, destination))
					{
						GameMemory newMemory = memory;

						if (newMemory.doMove(position, destination).isValid())
						{
							Node *node = new Node(this, newMemory, { position, destination }, !isMax, depth + 1, currentMin, currentMax);
							if(isMax)
							{
								currentValue = std::max(node->getHeuristic(), currentValue);
								currentMin = std::max(currentMin, currentValue);
							}
							else
							{
								currentValue = std::min(node->getHeuristic(), currentValue);
								currentMax = std::min(currentMax, currentValue);
							}
	
							children.push_back(std::make_unique<Node *>(node));

							if (currentMax <= currentMin)
							{
								return;
							}
						}
					}
					
					
					//tryMove(token, {dx, dy});
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
			//children.push_back(Node(this, newMemory, {position, destination}, !isMax, depth + 1, currentMin, currentMax));
		}
	}

}

Node *Node::getBestNode()
{

	Node *bestChild = nullptr;
	for(auto &child : children)
	{
		auto childPtr = *child;

		if(bestChild == nullptr)
		{
			bestChild = childPtr;
		}
		else
		{
			if(isMax)
			{
				if (bestChild->getHeuristic() < childPtr->getHeuristic())
				{
					bestChild = childPtr;
				}
			}
			else
			{
				if (bestChild->getHeuristic() > childPtr->getHeuristic())
				{
					bestChild = childPtr;
				}
			}
		}
	}

	return bestChild;
}

Node::Node(GameMemory memory):Node(nullptr, memory, {0,0}, memory.getCurrentTurn() == GREEN,0, INT_MIN, INT_MAX)
{
	std::cout << "Finding for " << (isMax?"MAX":"MIN") << std::endl;
}
