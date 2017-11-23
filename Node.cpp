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
		auto bestNode = findNextMoves();
		if (bestNode != nullptr) {
			heuristic = bestNode->heuristic;
			bestMove = bestNode->moveInfo;
		}
		else
		{
			calculateHeuristic();
		}
	}


}

void Node::calculateHeuristic()
{
	if(memory.getGreenPositions().size() == 0)
	{
		heuristic = INT_MIN;
	}else if(memory.getRedPositions().size() == 0)
	{
		heuristic = INT_MAX;
	}
	else
	{
		heuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();
	}
}

Node* Node::findNextMoves()
{
	auto &tokenList = memory.getCurrentTurnTokenList();

	int currentValue = isMax ? INT_MIN : INT_MAX;
	Node* bestNode = nullptr;

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
							children.push_back(std::make_unique<Node>(Node(this, newMemory, { position, destination }, !isMax, depth + 1, currentMin, currentMax)));
							auto &node = children.at(children.size() - 1);
							
							if(isMax)
							{
								if(node->getHeuristic() > currentValue)
								{
									currentValue = node->getHeuristic();
									bestNode = node.get();
								}
								currentMin = std::max(currentMin, currentValue);
							}
							else
							{
								if (node->getHeuristic() < currentValue)
								{
									currentValue = node->getHeuristic();
									bestNode = node.get();
								}
								currentMax = std::min(currentMax, currentValue);
							}

							if (currentMax <= currentMin)
							{
								return bestNode;
							}
						}
					}
				}
			}
		}
	}

	return bestNode;
}

Node::Node(GameMemory memory):Node(nullptr, memory, {0,0}, memory.getCurrentTurn() == GREEN,0, INT_MIN, INT_MAX)
{
	std::cout << "Finding for " << (isMax?"MAX":"MIN") << std::endl;
}
