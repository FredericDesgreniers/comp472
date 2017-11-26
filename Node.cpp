//
// Created by frede on 2017-11-02.
//

#include <iostream>
#include "Node.h"
#include <algorithm>
#include <queue>
#include <future>

int Node::totalNodes = 0;
int Node::totalEvaluates = 0;
int Node::totalPrunning = 0;

Node::Node(GameMemory& memory, MoveInfo moveInfo, bool isMax, int depth, int maxDepth): memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth), maxDepth(maxDepth)
{


	if(moveInfo.source.x >= 0)
	{
		auto moveResult = this->memory.doMoveUnsafe(moveInfo.source, moveInfo.destination);

		simpleHeuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();

		if (depth <= 3) {
			float percentOccupied = float(memory.getGreenPositions().size() + memory.getRedPositions().size()) / float(9 * 5);

			if (percentOccupied > 0.2 && percentOccupied < 0.8)
			{
				this->maxDepth = 6;
			}
		}
	}
#ifdef TRACK
	totalNodes++;
#endif
	
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
	int greenSize = memory.getGreenPositions().size();
	int redSize = memory.getRedPositions().size();

	heuristic = greenSize - redSize;
	//heuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();

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

	std::vector<std::shared_ptr<Node>> container;

	container.reserve(tokenList.size()*8);

	std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> childNodes(cmp, std::move(container));

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
						childNodes.push(std::make_shared<Node>(memory, MoveInfo(position, destination), !isMax, depth + 1, maxDepth));
					}
				}
			}
		}
	}


	std::shared_ptr<Node> bestNode;
	if (depth > 0)
	{
		while(!childNodes.empty()){

		
			std::shared_ptr<Node> node = childNodes.top();

			node->evaluate(currentMin, currentMax);

			if (evaluateForBestNode(node, bestNode, currentValue, currentMin, currentMax))
			{
				break;
			}

			childNodes.pop();
		}
		
	}
	else
	{
		while (!childNodes.empty())
		{
			auto node1 = childNodes.top();
			childNodes.pop();
			auto future1 = std::async(std::launch::async, &Node::evaluate, node1.get(), currentMin, currentMax);

			bool has2 = !childNodes.empty();

			if (has2) {
				auto node2 = childNodes.top();
				childNodes.pop();
				auto future2 = std::async(std::launch::async, &Node::evaluate, node2.get(), currentMin, currentMax);

				future2.wait();

				if (evaluateForBestNode(node2, bestNode, currentValue, currentMin, currentMax))
				{
					heuristic = bestNode->getHeuristic();
					bestMove = MoveInfo(bestNode->getMove());

					return;
				}
			}

			future1.wait();

			if (evaluateForBestNode(node1, bestNode, currentValue, currentMin, currentMax))
			{
				heuristic = bestNode->getHeuristic();
				bestMove = MoveInfo(bestNode->getMove());

				return;
			}

			
		}
	}
	heuristic = bestNode->getHeuristic();
	bestMove = MoveInfo( bestNode->getMove() );
}

bool Node::evaluateForBestNode(const std::shared_ptr<Node>& node, std::shared_ptr<Node>& bestNode, int& currentValue, int &currentMin, int &currentMax)
{
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
		return true;
	}

	return false;
}

Node::Node(GameMemory memory, int maxDepth):Node(memory, {-1,-1}, memory.getCurrentTurn() == GREEN,0, maxDepth)
{

}

