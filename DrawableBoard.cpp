
#include "DrawableBoard.h"

DrawableBoard::DrawableBoard(Board *board, vec2 position): board(board), position(position)
{

}

Board *DrawableBoard::getBoard()
{
	return board;
}

const vec2 DrawableBoard::getPixelPosition()
{
	return position * tileDimension;
}

const vec2 DrawableBoard::getPixelDimension()
{
	return board->getDimension() * tileDimension;
}


