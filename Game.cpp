#include <windows.h>

#include "Board.h"
#include "BoardRenderer.h"
#include "WindowsBoardRenderer.h"
#include <string>
#include <iostream>

bool wasMousePressed = false;
BoardRenderer *boardRenderer;

Tile *selectedTile = nullptr;

TileType currentTurn = GREEN;

/*
 * TODO All the code related to moving needs to be pulled out
 * Since it does not belong in the main game class
 *
 * Maybe in a Player class? Or the board class?
 */

//This propagates a move in a vertain direction.
// It returns true if it manages to kill at least one tile
// Returns false if not
bool propagateMoveInDirection(Board *board, const TileType originalType, const vec2 currentPosition, const vec2
direction)
{
	const vec2 positionInDirection = currentPosition + direction;
	Tile *tileAtNewPosition = board->getTileAt(positionInDirection);

	if(tileAtNewPosition != nullptr)
	{
		TileType tileTypeAtNewPosition = tileAtNewPosition->getType();
		if(tileTypeAtNewPosition != EMPTY && tileTypeAtNewPosition != originalType)
		{
			std::cout << positionInDirection.x << ", " << positionInDirection.y << std::endl;

			propagateMoveInDirection(board, originalType, positionInDirection, direction);
			board->setTileAt(positionInDirection, new Tile(positionInDirection, EMPTY, tileAtNewPosition->getIsBlack()));
			return true;
		}
	}

	return false;
}

//Try to do a move from source to destination
//TODO need to handle turns
void doMove(Board *board, const vec2 source, const vec2 destination)
{
	const vec2 direction = destination - source;

	if(abs(direction.x) > 1 || abs(direction.y) > 1)
	{
		//this move is not valid since the destination is too far away
		return;
	}

	bool diagonal = !((direction.x == 0) || (direction.y == 0));
	Tile *sourceTile = board->getTileAt(source);

	if(sourceTile->getType() != currentTurn)
	{
		return;
	}

	if(!sourceTile->getIsBlack() && diagonal)
	{
		//This move is not valid because it does diagonal on not black
		return;
	}

	//Try both forward and backward moves
	//Forward takes priority, backwards only gets tried when forward returns false
	if(!propagateMoveInDirection(board,sourceTile->getType(), destination, direction))
	{
		const vec2 oppositeDirection({-direction.x, -direction.y});
		propagateMoveInDirection(board, sourceTile->getType(), source, oppositeDirection);
	}

	currentTurn = currentTurn == GREEN?RED:GREEN;
	std::cout << "Current turn is now " << (currentTurn == GREEN?"green":"red") << std::endl;

	const vec2 newPosition = source + direction;

	//This swaps the tiles, which swaps the color and position
	Tile* newTileAtSource = board->getTileAt(newPosition);
	newTileAtSource->setPosition(source);
	bool blackAtNewPosition = newTileAtSource->getIsBlack();
	newTileAtSource->setIsBlack(sourceTile->getIsBlack());
	board->setTileAt(source, newTileAtSource);

	sourceTile->setPosition(newPosition);
	sourceTile->setIsBlack(blackAtNewPosition);
	board->setTileAt(newPosition, sourceTile);
}

/**
 * TODO Code below needs to be pulled out
 * Consider maybe an Input controller?
 */

//Gets called when a click event happens
void onClick(BoardRenderer *boardRenderer, HWND targetHandle)
{

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(targetHandle, &pt);

	const vec2 cursorPosition = {pt.x, pt.y};

	Tile* tile = boardRenderer->getTileAtDisplayCoordinates(cursorPosition);

	if(tile == nullptr)
	{
		return;
	}

	//selection logic
	//If the tile clicked is not empty, we select it
	//If the tile clicked is empty and a tile has been previously selected, we try to make a move

	if(tile->getType() != EMPTY)
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
		const vec2 destinationTile = boardRenderer->getTilePositionFromDisplayPosition(cursorPosition);

		if (destinationTile.x >= 0 && destinationTile.y >= 0)
		{
			doMove(boardRenderer->getDrawableBoard()->getBoard(), selectedTile->getPosition(),destinationTile);

			selectedTile->setIsSelected(false);
			selectedTile = nullptr;
		}
	}
}

/**
 * TODO All the code relating to the Windows api should be pulled out
 * Would make it simpler to change to an api like SDL later
 * Consider making an interface so we can just switch implementation
 */

LRESULT CALLBACK WndProc(HWND   hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT:
		{
			if(boardRenderer != nullptr)
			{
				boardRenderer->render();
			}
		}break;

		case WM_CLOSE:
		case WM_DESTROY:
		{
			exit(0);
		};

		case WM_LBUTTONDOWN:
		{
			if(!wasMousePressed)
			{
				wasMousePressed = true;
				onClick(boardRenderer, hwnd);
			}
		}break;

		case WM_LBUTTONUP:
		{
			wasMousePressed = false;
		}break;

		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return 0;
}

void iniGame(HWND windowHandle)
{
	const vec2 boardPosition = {20, 20};
	const vec2 boardDimension = {9, 5};

	Board *board = new Board(boardDimension);
	DrawableBoard *drawableBoard = new DrawableBoard(board, boardPosition);

	boardRenderer = new WindowsBoardRenderer(drawableBoard, windowHandle);

	std::cout << "Current turn is now " << (currentTurn == GREEN?"green":"red") << std::endl;
}

void runWindowMessageLoop(HWND windowHandle)
{
	MSG msg;
	while (GetMessage(&msg, windowHandle, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS windowClass = {};

	windowClass.style          = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc    = WndProc;
	windowClass.cbClsExtra     = 0;
	windowClass.cbWndExtra     = 0;
	windowClass.hInstance      = hInstance;
	windowClass.hIcon          = NULL;
	windowClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	windowClass.lpszMenuName   = NULL;
	windowClass.lpszClassName  = "game_test_123";

	if (!RegisterClass(&windowClass))
	{
		MessageBox(NULL,
		           ("Call to RegisterClassEx failed!"),
		           ("Game"),
		           NULL);

		return 1;
	}
	HWND windowHandle = CreateWindow(
			windowClass.lpszClassName,
			"Game test",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			600, 400,
			NULL,
			NULL,
			hInstance,
			NULL
	);
	if (!windowHandle)
	{

		MessageBox(NULL,
		           ("Call to CreateWindow failed! "),
		           ("Game"),
		           NULL);

		return 1;
	}

	ShowWindow(windowHandle, true);

	iniGame(windowHandle);

	runWindowMessageLoop(windowHandle);

	return 0;
}