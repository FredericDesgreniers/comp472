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

	const bool getIsSelected() const {return isSelected;}
	void setIsSelected(bool isSelected){this->isSelected = isSelected;}

	const TileType getType() const {return type;} ;

	const vec2 getPosition() const {return position;}

	const bool getIsBlack() const {return isBlack;}
};




