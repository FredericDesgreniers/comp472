#pragma once

class Tile
{
private:
	int x;
	int y;

public:
	Tile(int x, int y);

	bool isSelected = false;

	int getX() const;

	int getY() const;
};




