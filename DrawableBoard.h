

#pragma once


#include "Board.h"

class DrawableBoard
{
private:
	Board *board;

	vec2 position;

	vec2 tileDimension = {60,60};

public:
	DrawableBoard(Board *board, vec2 position);

	const vec2 getPixelPosition();

	const vec2 getPixelDimension();

	Board *getBoard();

	const vec2 getPosition(){return position;}
	void setPosition(vec2 position){this->position = position;};

	const vec2 getTileDimension(){return tileDimension;}
	void setTileDimension(vec2 tileDimension){this->tileDimension= tileDimension;};

};




