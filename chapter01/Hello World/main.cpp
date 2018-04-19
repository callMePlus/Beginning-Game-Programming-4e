// Beginning Game Programming, 4th Edition
// Chapter 1 - HelloWorld program

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nShowCmd)
{
    MessageBox(NULL, "Welcome to Windows Programming!",
		"HELLO WORLD", MB_OK | MB_ICONEXCLAMATION);
}

