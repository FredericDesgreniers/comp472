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

	bool getIsSelected() const {return isSelected;}
	void setIsSelected(bool isSelected){this->isSelected = isSelected;}

	TileType getType() const {return type;} ;

	vec2 getPosition() const {return position;}

	bool getIsBlack() const {return isBlack;}
};




