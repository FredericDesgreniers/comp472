#include <iostream>
#include <windows.h>
#include "GameMemory.h"
#include "BoardRenderer.h"
#include "WindowsBoardRenderer.h"

bool wasMousePressed = false;
vec2 *selectedTilePosition = nullptr;
BoardRenderer* boardRenderer;

/**
 * TODO All the code relating to the Windows api should be pulled out
 * Would make it simpler to change to an api like SDL later
 * Consider making an interface so we can just switch implementation
 */

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		case WM_PAINT:
		{

				if(boardRenderer != nullptr)
			boardRenderer->render();
		}break;

		case WM_CLOSE:
		case WM_DESTROY:
		{
			exit(0);
		};

		case WM_LBUTTONDOWN:
		{
			if (!wasMousePressed)
			{
				wasMousePressed = true;
				//onClick(boardRenderer, hwnd);
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

	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = NULL;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "game_test_123";

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

	GameMemory memory;

	boardRenderer = new WindowsBoardRenderer(memory.getTileArray(), {9, 5}, windowHandle);
	//iniGame(windowHandle);

	runWindowMessageLoop(windowHandle);

	return 0;
}