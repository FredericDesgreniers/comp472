#pragma once

#include <windows.h>

class DoubleBuffer
{
	HWND primaryBufferHandle;
	HDC primaryBufferDC;

	HDC secondaryBufferDC;

	HBITMAP bmp;
	HBITMAP bmpold;

public:
	DoubleBuffer(HWND handleTarget);

	void allocateBuffer();

	void swapBuffers();

	HWND getHandle()
	{
		return primaryBufferHandle;
	}
	HDC getUsableDC()
	{
		return secondaryBufferDC;
	}
};




