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

Node::Node(GameMemory& memory, MoveInfo moveInfo, bool isMax, int depth): memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depth(depth)
{
	if(moveInfo.source.x >= 0)
	{
		this->memory.doMoveUnsafe(moveInfo.source, moveInfo.destination);
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

	std::vector<std::shared_ptr<Node>> container;
	container.reserve(tokenList.size()*2);

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
						childNodes.push(std::make_shared<Node>(memory, MoveInfo(position, destination), !isMax, depth + 1));
					}
				}
			}
		}
	}

	std::shared_ptr<Node> bestNode;
	if (depth > 2)
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
		while (!childNodes.empty()) {
			int threadNum = 2;
			std::vector<std::shared_ptr<Node>> nodes;
			nodes.reserve(threadNum);
			std::vector<std::future<void>> nodeFutures;
			nodeFutures.reserve(threadNum);

			for (int i = 0; i < threadNum; i++)
			{
				if (!childNodes.empty())
				{
					std::shared_ptr<Node> node = childNodes.top();
					childNodes.pop();
					nodes.push_back(node);
					nodeFutures.push_back(std::async(std::launch::async, &Node::evaluate, node.get(), currentMin, currentMax));
				}

			}

			int currentNode = 0;
			for (auto &f : nodeFutures)
			{
				f.wait();
				if (evaluateForBestNode(nodes.at(currentNode++), bestNode, currentValue, currentMin, currentMax))
				{
					heuristic = bestNode->getHeuristic();
					bestMove = MoveInfo(bestNode->getMove());

					return;
				}

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

Node::Node(GameMemory memory):Node(memory, {-1,-1}, memory.getCurrentTurn() == GREEN,0)
{
	std::cout << "Finding for " << (isMax?"MAX":"MIN") << std::endl;
}

