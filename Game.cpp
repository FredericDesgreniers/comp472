#include <iostream>
#define _WIN32_WINNT 0x500
#include "Board.h"
#include "BoardRenderer.h"

int main()
{
	Board board(9,5);

	HWND consoleHandle = GetConsoleWindow();
	HDC consoleDC = GetDC(consoleHandle);

	BoardRenderer boardRenderer(board, consoleDC);
	boardRenderer.render(100, 100);

	std::cin.get();
}