//
// Created by frede on 2017-11-02.
//

#include "Node.h"

Node::Node(GameMemory memory, int depth): memory(memory), depth(depth)
{
	calculateHeuristic();
	if (depth < 3)
	{
		findNextMoves();
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
		for(int dx = 0; dx <= 1; dx++)
		{
			for(int dy = 0; dy <= 1; dy++)
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
	GameMemory newMemory = memory;
	if(memory.doMove(position, position + direction).isValid())
	{
		children.push_back(Node(newMemory, depth+1));
	}
}
