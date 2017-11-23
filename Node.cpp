//
// Created by frede on 2017-11-02.
//

#include <iostream>
#include "Node.h"
#include <algorithm>
#include <queue>

int Node::totalNodes = 0;
int Node::totalEvaluates = 0;
int Node::totalPrunning = 0;

Node::Node(Node *parent, GameMemory memory, MoveInfo moveInfo, bool isMax, int depth): parent(parent), memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth)
{
	totalNodes++;
	simpleHeuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();
}

void Node::evaluate(int currentMin, int currentMax)
{
	totalEvaluates++;

	heuristic = 0;
	if (depth == maxDepth)
	{
		calculateHeuristic();
	}
	else if (depth < maxDepth)
	{
		bestNode = findBestNode(currentMin, currentMax);
		if (bestNode) {
			heuristic = bestNode->heuristic;
			bestMove = bestNode->moveInfo;
		}
		else
		{
			calculateHeuristic();
			bestMove = MoveInfo{ { 0 },{ 0 } };
		}
	}
}


void Node::calculateHeuristic()
{
	heuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();

}

std::shared_ptr<Node> Node::findBestNode(int currentMin, int currentMax)
{
	auto &tokenList = memory.getCurrentTurnTokenList();

	std::shared_ptr<Node> bestNode;
	//current best node
	bool shouldBeMax = isMax;
	auto cmp = [shouldBeMax](std::shared_ptr<Node> &left, std::shared_ptr<Node> &right) {if(shouldBeMax){return left->simpleHeuristic < right->simpleHeuristic; } else {return right->simpleHeuristic < left->simpleHeuristic; }
};
	//std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> childNodes(cmp);
	std::vector<std::shared_ptr<Node>> childNodes;
	//current min/max value
	int currentValue = isMax ? INT_MIN : INT_MAX;

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
							childNodes.push_back(std::make_shared<Node>(this, newMemory, MoveInfo(position, destination), !isMax, depth + 1));
						}
					}
				}
			}
		}
	}
	
	std::sort(childNodes.begin(), childNodes.end(), [this](std::shared_ptr<Node> &left, std::shared_ptr<Node> &right)
	{
		if (!isMax) {
			return left->simpleHeuristic < right->simpleHeuristic;
		}
		else
		{
			return right->simpleHeuristic < left->simpleHeuristic;
		}

	});

	for(auto node : childNodes)
	{
		node->evaluate(currentMin, currentMax);
		

		//Min max alghrithm
		//see https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning#Pseudocode
		if (isMax)
		{
			if (node->getHeuristic() > currentValue)
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
			totalPrunning++;
			return bestNode;
		}
	}

	return bestNode;
}

Node::Node(GameMemory memory):Node(nullptr, memory, {0,0}, memory.getCurrentTurn() == GREEN,0)
{
	std::cout << "Finding for " << (isMax?"MAX":"MIN") << std::endl;
}

