#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// program values
const string APPTITLE = "Draw Bitmap Program";
const int SCREENW = 1024;
const int SCREENH = 768;
// key macro
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// Direct3D object
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPDIRECT3DSURFACE9 surface = NULL;

bool gameover = false;

bool Game_Init(HWND window){
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		MessageBox(window, "Error initializing Direct3D", "Error", MB_OK);
		return false;
	}

	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	srand((unsigned int)time(NULL));
	//d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREENW;
	d3dpp.BackBufferHeight = SCREENH;
	d3dpp.hDeviceWindow = window;

	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &d3ddev);
	if (!d3ddev)
	{
		MessageBox(window, "Error creating Direct3D device", "Error", MB_OK);
		return false;
	}
	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	HRESULT result = d3ddev->CreateOffscreenPlainSurface(
		SCREENW,
		SCREENH,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&surface,
		NULL
		);
	if (result != D3D_OK) return false;
	result = D3DXLoadSurfaceFromFile(
		surface,
		NULL,
		NULL,
		"photo.png",
		NULL,
		D3DX_DEFAULT,
		0,
		NULL
		);
	if (result != D3D_OK) return false;
	return true;
}
void Game_Run(HWND hwnd)
{
	//make sure the Direct3D device is valid
	if (!d3ddev) return;
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	//start rendering
	if (d3ddev->BeginScene())
	{
		d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		RECT rect;
		rect.left = rand() % SCREENW;
		rect.right = rect.left + rand() % (SCREENW - rect.left);
		rect.top = rand() % SCREENH;
		rect.bottom = rect.top + rand() % (SCREENH - rect.top);

		d3ddev->StretchRect(surface, &rect, backbuffer, &rect, D3DTEXF_NONE);
		//stop rendering
		d3ddev->EndScene();

		//display the back buffer on the screen
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	//check for escape key (to exit program)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}
void Game_End(HWND hwnd)
{
	if (surface) {
		surface->Release();
		surface = NULL;
	}
	if (d3ddev) {
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d) {
		d3d->Release();
		d3d = NULL;
	}
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Windows entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//create the window class structure
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
	wc.lpszClassName = "MainWindowClass";
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	//create a new window
	HWND window = CreateWindow("MainWindowClass", APPTITLE.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		SCREENW, SCREENH, NULL, NULL, hInstance, NULL);

	//was there an error creating the window?
	if (window == 0) return 0;

	//display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//initialize the game
	if (!Game_Init(window)) return 0;


	// main message loop
	MSG message;
	while (!gameover)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		Game_Run(window);
	}

	return message.wParam;
}






