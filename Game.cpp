#include <iostream>
#include <unistd.h>
#include <time.h>

#include "Board.h"
#include "BoardRenderer.h"
#include "ConsoleBoardRenderer.h"

int main()
{
	Board board(9,5);

	HWND consoleHandle = GetConsoleWindow();
	HDC consoleDC = GetDC(consoleHandle);


	BoardRenderer *boardRenderer = new ConsoleBoardRenderer(board, consoleDC);
	boardRenderer->render(100, 100);

	while(1)
	{
		sleep(1);
		boardRenderer->render(100, 100);
	}

}