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

	DrawableBoard drawableBoard(&board, 100, 100);

	BoardRenderer *boardRenderer = new ConsoleBoardRenderer(drawableBoard, consoleHandle);


	bool running = true;

	while(running)
	{
		Sleep(200);
		boardRenderer->render();
	}

}