#include <windows.h>

#include "Board.h"
#include "BoardRenderer.h"
#include "WindowsBoardRenderer.h"
#include <string>

bool wasMousePressed = false;
BoardRenderer *boardRenderer;

Tile *selectedTile = nullptr;

void doMove(Board *board, int sourceX, int sourceY, int destinationX, int destinationY)
{

}

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

	if(!(tile->getType() == EMPTY))
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
			doMove(boardRenderer->getDrawableBoard()->getBoard(), selectedTile->getPosition().x,
			       selectedTile->getPosition().y,
			       destinationTile.x, destinationTile.y);

			selectedTile->setIsSelected(false);
			selectedTile = nullptr;
		}
	}
}

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