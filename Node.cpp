//
// Created by frede on 2017-11-02.
//

#include <iostream>
#include "Node.h"
#include <algorithm>
#include <queue>
#include <future>

unsigned int Node::totalNodes = 0;
unsigned int Node::totalEvaluates = 0;
unsigned int Node::totalPrunning = 0;

Node::Node(GameMemory& memory, MoveInfo moveInfo, bool isMax, DepthInfo depthInfo): memory(memory)
		, moveInfo(moveInfo), isMax(isMax), depthInfo(depthInfo)
{


	if(moveInfo.source.x >= 0)
	{
		auto moveResult = this->memory.doMoveUnsafe(moveInfo.source, moveInfo.destination);

		simpleHeuristic = memory.getGreenPositions().size() - memory.getRedPositions().size();

		if (depthInfo.depth <= 3) {
			float percentOccupied = float(memory.getGreenPositions().size() + memory.getRedPositions().size()) / float(9 * 5);

			if (percentOccupied > 0.2 && percentOccupied < 0.8)
			{
				this->depthInfo.maxDepth = 5;
			}
		}

	}
#ifdef TRACK
	totalNodes++;
#endif
	
}

int Node::evaluateHeuristic(const vec2& position, const vec2& direction)
{

	auto nextPosition = position + direction;
	auto nextTile = memory.getTileAt(nextPosition);
	auto secondNextTile = memory.getTileAt(nextPosition + direction);
	auto behindTile = memory.getTileAt(position - direction);
	auto sourceTile = memory.getTileAt(position);

	auto enemyTile = getOpposite(sourceTile);

	if (nextTile == INVALID)
	{
		return 0;
	}
	else
	{
		if (nextTile == sourceTile)
		{
			if (secondNextTile == sourceTile)
			{
				return -20;
			}
			return -10;
		}
		else if (nextTile == EMPTY)
		{
			if(secondNextTile == enemyTile || behindTile == enemyTile)
			{
				if(isMax)
				{
					if(sourceTile == GREEN)
					{
						return 10;
					}
					else //RED
					{
						return -10;
					}
				}
				else
				{
					if (sourceTile == GREEN)
					{
						return -10;
					}
					else //RED
					{
						return 10;
					}
				}
			}
		}
		else if(nextTile == enemyTile)
		{
			if(isMax)
			{
				if(sourceTile == GREEN)
				{
					if (behindTile == EMPTY)
					{
						return 5;
					}
					else
					{
						return 0;
					}
				}
				else //RED
				{
					if(secondNextTile == EMPTY)
					{
						return 5;
					}
				}
				
			}
			else
			{
				if(sourceTile == GREEN)
				{
					if(secondNextTile == EMPTY)
					{
						return -5;
					}
					else
					{
						return 0;
					}
				}
				else // RED
				{
					if(behindTile == EMPTY)
					{
						return -5;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}

	return 0;
}

void Node::evaluate(int currentMin, int currentMax)
{
#ifdef TRACK
	totalEvaluates++;
#endif


	if (depthInfo.depth >= depthInfo.maxDepth)
	{
		calculateHeuristic();
	}

	if (depthInfo.depth < depthInfo.maxDepth)
	{
		findBestNode(currentMin, currentMax);
	}
}



void Node::calculateHeuristic()
{
	heuristic = 0;

	for(auto &tile : memory.getGreenPositions())
	{
		int tileHeuristic = 105;

		tileHeuristic += evaluateHeuristic(tile, vec2{ -1, 0 });
		tileHeuristic += evaluateHeuristic(tile, vec2{ 1, 0 });
		tileHeuristic += evaluateHeuristic(tile, vec2{ 0, -1 });
		tileHeuristic += evaluateHeuristic(tile, vec2{ 0, 1 });

		if(isBlackReferenceBoard[tile.y][tile.x])
		{
			tileHeuristic += evaluateHeuristic(tile, vec2{ -1, -1 });
			tileHeuristic += evaluateHeuristic(tile, vec2{ -1, 1 });
			tileHeuristic += evaluateHeuristic(tile, vec2{ 1, -1 });
			tileHeuristic += evaluateHeuristic(tile, vec2{ 1, 1 });
		}

		heuristic += tileHeuristic;
	}

	for (auto &tile : memory.getRedPositions())
	{
		int tileHeuristic = -105;

		tileHeuristic += evaluateHeuristic(tile, vec2{ -1, 0 });
		tileHeuristic += evaluateHeuristic(tile, vec2{ 1, 0 });
		tileHeuristic += evaluateHeuristic(tile, vec2{ 0, -1 });
		tileHeuristic += evaluateHeuristic(tile, vec2{ 0, 1 });

		if (isBlackReferenceBoard[tile.y][tile.x])
		{
			tileHeuristic += evaluateHeuristic(tile, vec2{ -1, -1 });
			tileHeuristic += evaluateHeuristic(tile, vec2{ -1, 1 });
			tileHeuristic += evaluateHeuristic(tile, vec2{ 1, -1 });
			tileHeuristic += evaluateHeuristic(tile, vec2{ 1, 1 });
		}

		heuristic += tileHeuristic;
	}

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
						childNodes.push(std::make_shared<Node>(memory, MoveInfo(position, destination), !isMax, DepthInfo{ depthInfo.depth + 1, depthInfo.maxDepth }));
					}
				}
			}
		}
	}


	std::shared_ptr<Node> bestNode;
	if (depthInfo.depth > 0)
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

Node::Node(GameMemory memory,unsigned int maxDepth):Node(memory, {-1,-1}, memory.getCurrentTurn() == GREEN, { 0, maxDepth })
{

}

