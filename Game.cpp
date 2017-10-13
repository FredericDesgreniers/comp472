#include <windows.h>
#include <tchar.h>
#include "Board.h"
#include "BoardRenderer.h"
#include "ConsoleBoardRenderer.h"
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

LRESULT CALLBACK WndProc(HWND   hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT:
		{
			boardRenderer->render();
		}break;

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
		           _T("Call to RegisterClassEx failed!"),
		           _T("Game"),
		           NULL);

		return 1;
	}
	HWND hWnd = CreateWindow(
			windowClass.lpszClassName,
			"Game test",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			1000, 500,
			NULL,
			NULL,
			hInstance,
			NULL
	);
	if (!hWnd)
	{

		MessageBox(NULL,
		           _T("Call to CreateWindow failed! "),
		           _T("Game"),
		           NULL);

		return 1;
	}

	ShowWindow(hWnd, true);
	Board board(9,5);

	DrawableBoard drawableBoard(&board, 20, 20);

	boardRenderer = new ConsoleBoardRenderer(drawableBoard, hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}