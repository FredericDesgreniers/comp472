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
	void setType(TileType type) {this->type = type;}

	vec2 getPosition() const {return position;}

	void setPosition(vec2 position) {this->position = position;}

	bool getIsBlack() const {return isBlack;}
	void setIsBlack(bool isBlack){this->isBlack = isBlack;}
};




