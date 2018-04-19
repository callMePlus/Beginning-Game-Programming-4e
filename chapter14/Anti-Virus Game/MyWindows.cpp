// Beginning Game Programming
// MyWindows.cpp

#include "MyDirectX.h"
using namespace std;
bool gameover = false;
extern const bool FULLSCREEN;

LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            gameover = true;
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd=0;

    //initialize window settings
    wc.cbSize = sizeof(WNDCLASSEX); 
    wc.style         = 0;// CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinProc;
    wc.cbClsExtra	 = 0;
    wc.cbWndExtra	 = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.lpszMenuName = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = "MainWindowClass";

    if (!RegisterClassEx(&wc))
        return FALSE;

    //create a new window
    if (FULLSCREEN == TRUE)
    {
        hwnd = CreateWindow("MainWindowClass", APPTITLE.c_str(),
            WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
            SCREENW, SCREENH, (HWND)NULL,
            (HMENU)NULL, hInstance, (LPVOID)NULL);
    }
    else {
        hwnd = CreateWindow("MainWindowClass", APPTITLE.c_str(),
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            SCREENW, SCREENH, NULL, NULL, hInstance, NULL);
    }

    if (hwnd == 0) return 0;

    //display the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
	
	//initialize the game
    if (!Game_Init(hwnd)) return 0;

    // main message loop
	MSG message;
	while (!gameover)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) 
	    {
		    TranslateMessage(&message);
		    DispatchMessage(&message);
	    }

        //process game loop 
        Game_Run(hwnd);
    }

    //shutdown
    Game_End();
	return message.wParam;
}

