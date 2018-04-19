// Beginning Game Programming
// Chapter 2 - WindowTest program

#include <windows.h>
#include <iostream>
using namespace std;
const string ProgramTitle = "Hello Windows";

// The window event callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT drawRect;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps); //start drawing 
		for (int n = 0; n < 20; n++)
		{
			int x = n * 20;
			int y = n * 20;
			drawRect = { x, y, x+100, y+20 };
			DrawText(hdc, ProgramTitle.c_str(), ProgramTitle.length(), &drawRect, DT_CENTER);
		}
		EndPaint(hWnd, &ps); //stop drawing
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Helper function to set up the window properties
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//set the new window's properties
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ProgramTitle.c_str();
	wc.hIconSm = NULL;
	return RegisterClassEx(&wc);
}


// Helper function to create the window and refresh it
bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    //create a new window
    HWND hWnd = CreateWindow(
        ProgramTitle.c_str(),        //window class
        ProgramTitle.c_str(),        //title bar
        WS_OVERLAPPEDWINDOW,         //window style
        CW_USEDEFAULT, CW_USEDEFAULT, //position of window
        640, 480,                    //dimensions of the window
        NULL,                        //parent window (not used)
        NULL,	                        //menu (not used)
        hInstance,                   //application instance
        NULL);                       //window parameters (not used)

    //was there an error creating the window?
    if (hWnd == 0) return 0;

    //display the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return 1;
}

// Entry point for a Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//create the window
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return 0;

	// main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
