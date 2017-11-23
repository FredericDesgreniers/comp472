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

Node::Node(GameMemory memory, MoveInfo moveInfo, bool isMax, int depth): memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth)
{
	if(moveInfo.source.x >= 0)
	{
		memory.doMoveUnsafe(moveInfo.source, moveInfo.destination);
	}
#ifdef TRACK
	totalNodes++;
#endif
	simpleHeuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();
}

void Node::evaluate(int currentMin, int currentMax)
{
#ifdef TRACK
	totalEvaluates++;
#endif

	if (depth == maxDepth)
	{
		calculateHeuristic();
	}
	else if(memory.getGreenPositions().size() == 0)
	{
		heuristic = INT_MIN/depth;
	}
	else if(memory.getRedPositions().size() == 0)
	{
		heuristic = INT_MAX/depth;
	}
	else if (depth < maxDepth)
	{
		findBestNode(currentMin, currentMax);
	}
}


void Node::calculateHeuristic()
{
	heuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();

}

void Node::findBestNode(int currentMin, int currentMax)
{
	auto &tokenList = memory.getCurrentTurnTokenList();

	

	//current best node
	auto cmp = [this](std::shared_ptr<Node> &left, std::shared_ptr<Node> &right) {
		if(isMax)
		{
			return left->simpleHeuristic < right->simpleHeuristic;
		} 
		else
		{
			return right->simpleHeuristic < left->simpleHeuristic;
		}
	};

	std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> childNodes(cmp);

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
						childNodes.push(std::make_shared<Node>(memory, MoveInfo(position, destination), !isMax, depth + 1));
					}
				}
			}
		}
	}

	std::shared_ptr<Node> bestNode;

	while(!childNodes.empty()){
		const std::shared_ptr<Node> &node = childNodes.top();
		
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
#ifdef TRACK
			totalPrunning++;
#endif
			break;
		}
		childNodes.pop();
	}
	heuristic = bestNode->getHeuristic();
	bestMove = MoveInfo( bestNode->getMove() );
}

Node::Node(GameMemory memory):Node(memory, {-1,-1}, memory.getCurrentTurn() == GREEN,0)
{
	std::cout << "Finding for " << (isMax?"MAX":"MIN") << std::endl;
}

