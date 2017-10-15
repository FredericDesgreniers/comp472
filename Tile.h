#pragma once

#include "TileType.h"
#include "vec2.h"

class Tile
{
private:
	vec2 position;
	bool isSelected = false;
	bool isBlack;

	TileType type;
public:
	Tile(vec2 position, TileType type, bool isBlack);

	bool getIsSelected(){return isSelected;}
	void setIsSelected(bool isSelected){this->isSelected = isSelected;}

	TileType getType(){return type;}

	vec2 getPosition(){return position;}

	bool getIsBlack() {return isBlack;}
};




