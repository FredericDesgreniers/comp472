#include <iostream>
#include "Board.h"
#include "BoardRenderer.h"
#include "ConsoleBoardRenderer.h"

bool wasMousePressed = false;

bool isMousePressed()
{
	return (GetKeyState(VK_LBUTTON) & 0x100) !=  0;
}

Tile *selectedTile = nullptr;

void doMove(Board *board, int sourceX, int sourceY, int destinationX, int destinationY)
{

}

void onClick(BoardRenderer *boardRenderer, HWND targetHandle)
{

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(targetHandle, &pt);

	Tile* tile = boardRenderer->getTileAtDisplayCoordinates(pt.x, pt.y);

	if(tile == nullptr)
	{
		return;
	}

	if(!tile->getIsEmpty())
	{
		if(selectedTile != nullptr)
		{
			selectedTile->setIsSelected(false);
		}

		selectedTile = tile;
		selectedTile->setIsSelected(true);
	}
	else if(selectedTile != nullptr)
	{
		int destinationTileX = boardRenderer->getTilePosXFromDisplayCoordinates(pt.x);
		int destinationTileY = boardRenderer->getTilePosYFromDisplayCoordinates(pt.y);

		if (destinationTileX >= 0 && destinationTileY >= 0)
		{
			doMove(boardRenderer->getDrawableBoard().getBoard(), selectedTile->getX(), selectedTile->getY(),
			       destinationTileX, destinationTileY);

			selectedTile->setIsSelected(false);
			selectedTile = nullptr;
		}
	}
}

int main()
{
	Board board(9,5);

	HWND consoleHandle = GetConsoleWindow();

	DrawableBoard drawableBoard(&board, 20, 20);

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
		Sleep(20);
	}

}