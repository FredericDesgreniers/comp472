#pragma once

class Tile
{
private:
	int x;
	int y;
	bool isSelected = false;

	bool isEmpty = true;
public:
	Tile(int x, int y, bool isEmpty);

	bool getIsSelected(){return isSelected;}
	void setIsSelected(bool isSelected){this->isSelected = isSelected;}

	bool getIsEmpty(){return isEmpty;}

	int getX() const;

	int getY() const;
};




