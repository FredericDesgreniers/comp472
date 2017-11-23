//
// Created by frede on 2017-11-02.
//

#include <iostream>
#include "Node.h"
#include <algorithm>

Node::Node(Node *parent, GameMemory &memory, MoveInfo moveInfo, bool isMax, int depth, int currentMin, int currentMax): parent(parent), memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth), currentMin(currentMin), currentMax(currentMax)
{
	heuristic = 0;
	if(depth == maxDepth)
	{
		calculateHeuristic();
	}
	else if (depth < maxDepth)
	{
		bestNode = findBestNode();
		if (bestNode) {
			heuristic = bestNode->heuristic;
			bestMove = bestNode->moveInfo;
		}
		else
		{
			calculateHeuristic();
			bestMove = MoveInfo{ {0},{0}};
		}
	}


}

void Node::calculateHeuristic()
{
	heuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();

}

std::shared_ptr<Node> Node::findBestNode()
{
	auto &tokenList = memory.getCurrentTurnTokenList();

	//current min/max value
	int currentValue = isMax ? INT_MIN : INT_MAX;

	std::shared_ptr<Node> bestNode;
	//current best node


	for(const auto &token : tokenList)
	{
		//Look at moves in a square around the token position
		for(int dx = -1; dx <= 1; dx++)
		{
			for(int dy = -1; dy <= 1; dy++)
			{
				if(dx != 0 || dy != 0)
				{
					auto position = token;
					auto direction = vec2{ dx, dy };

					auto destination = position + direction;
					if (memory.isValidMove(position, destination))
					{
						GameMemory newMemory = memory;

						if (newMemory.doMove(position, destination).isValid())
						{
							auto node = std::make_shared<Node>(Node(this, newMemory, { position, destination }, !isMax, depth + 1, currentMin, currentMax));
							
							//make sure best node is something

							
							//Min max alghrithm
							//see https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning#Pseudocode
							if(isMax)
							{
								if(node->getHeuristic() > currentValue)
								{
									currentValue = node->getHeuristic();
									bestNode = node;
								}
								currentMin = std::max(currentMin, currentValue);
							}
							else
							{
								if (node->getHeuristic() < currentValue)
								{
									currentValue = node->getHeuristic();
									bestNode = node;
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

Node::Node(GameMemory &memory):Node(nullptr, memory, {0,0}, memory.getCurrentTurn() == GREEN,0, INT_MIN, INT_MAX)
{
	std::cout << "Finding for " << (isMax?"MAX":"MIN") << std::endl;
}
