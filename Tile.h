#pragma once

#include "TileType.h"

class Tile
{
private:
	int x;
	int y;
	bool isSelected = false;

	TileType type;
public:
	Tile(int x, int y, TileType type);

	bool getIsSelected(){return isSelected;}
	void setIsSelected(bool isSelected){this->isSelected = isSelected;}

	TileType getType(){return type;}

	int getX() const;

	int getY() const;
};




