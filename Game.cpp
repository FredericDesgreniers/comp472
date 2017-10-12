#include <iostream>
#include "Board.h"

int main()
{
	Board board(9,5);
	for(int y = 0; y < board.getHeight(); y++)
	{
		for (int x = 0; x < board.getWidth(); x++)
		{
			Tile *tile = board.getTileAt(x, y);
			if(tile == nullptr)
			{
				std::cout << "n";
			}
			else
			{
				std::cout << "t";
			}
		}

		std::cout << std::endl;

	}

	system("pause");
}