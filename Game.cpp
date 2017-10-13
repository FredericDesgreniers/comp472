#include <iostream>
#include "Board.h"
#include "BoardRenderer.h"
#include "ConsoleBoardRenderer.h"

bool wasMousePressed = false;

bool isMousePressed()
{
	return (GetKeyState(VK_LBUTTON) & 0x100) !=  0;
}

void onClick(BoardRenderer *boardRenderer, HWND targetHandle)
{

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(targetHandle, &pt);

	Tile* tile = boardRenderer->getTileAtDisplayCoordinates(pt.x, pt.y);

	if(tile != nullptr)
	{
		tile->isSelected = true;
	}
}

int main()
{
	Board board(9,5);

	HWND consoleHandle = GetConsoleWindow();

	DrawableBoard drawableBoard(&board, 100, 100);

	BoardRenderer *boardRenderer = new ConsoleBoardRenderer(drawableBoard, consoleHandle);


	bool running = true;

	while(running)
	{
		if(isMousePressed()){
			if(!wasMousePressed)
			{
				wasMousePressed = true;
				onClick(boardRenderer, consoleHandle);
			}
		}
		else
		{
			wasMousePressed = false;
		}
		boardRenderer->render();
	}

}