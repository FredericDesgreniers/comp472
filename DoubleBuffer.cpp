
#include "DoubleBuffer.h"

DoubleBuffer::DoubleBuffer(HWND primaryBufferHandle): primaryBufferHandle(primaryBufferHandle)
{
	primaryBufferDC = GetDC(primaryBufferHandle);
}

void DoubleBuffer::allocateBuffer()
{
	secondaryBufferDC = CreateCompatibleDC(NULL);

	RECT primaryBufferRect;
	GetWindowRect(primaryBufferHandle, &primaryBufferRect);

	bmp = CreateCompatibleBitmap(primaryBufferDC, primaryBufferRect.right, primaryBufferRect.bottom);
	bmpold = (HBITMAP)SelectObject(secondaryBufferDC, bmp);

	BitBlt(secondaryBufferDC, 0, 0,primaryBufferRect.right, primaryBufferRect.bottom,
	       0, 0, 0, WHITENESS);
}

void DoubleBuffer::swapBuffers()
{
	RECT primaryBufferRect;
	GetWindowRect(primaryBufferHandle, &primaryBufferRect);

	BitBlt(primaryBufferDC, 0, 0,primaryBufferRect.right, primaryBufferRect.bottom,
	       secondaryBufferDC, 0, 0, SRCCOPY);



	SelectObject(secondaryBufferDC, bmpold);
	DeleteObject(bmp);
	DeleteObject(secondaryBufferDC);
}




