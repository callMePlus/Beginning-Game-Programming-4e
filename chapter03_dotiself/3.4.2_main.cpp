#include<windows.h>
#include <d3d9.h>
#include <time.h>
#include <iostream>
using namespace std;
const string APPTITLE = "Direct3D_Windowed";
const int SCREENW = 640;
const int SCREENH = 480;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
bool gameover = false;

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? 1 : 0)

bool Game_Init(HWND hwnd){
	// 初始化Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL){
		MessageBox(hwnd,"Error initialzing Direct3D","Error",MB_OK);
		return FALSE;
	}

	D3DDISPLAYMODE dm;
	// 用于动态获取屏幕的信息
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dm);
	// 用于设置设备参数
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = dm.Format;//D3DFMT_X8R8G8B8
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREENW;// dm.Width;
	d3dpp.BackBufferHeight = SCREENH;//dm.Height;
	d3dpp.hDeviceWindow = hwnd;
	// 创建输出现实内容的设备
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, // 设置的参数
		&d3ddev // 设备指针
		);
	if (d3ddev == NULL){
		MessageBox(hwnd, "Error creating Direct3D device", "Error", MB_OK);
		return FALSE;
	}
	return true;
}


void Game_Run(HWND hwnd) {
	if (!d3ddev) return;
	//r g b
	d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,255,0),1.0f,0);
	if (d3ddev->BeginScene()) {
		// do something
		// stop rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL,NULL,NULL,NULL);
	}
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}

void Game_End(HWND hwnd){
	if (d3ddev){
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d){
		d3d->Release();
		d3d = NULL;
	}
}

LRESULT WINAPI WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM pParam){
	switch (msg){
	case WM_DESTROY:
		gameover = true;
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, pParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance){
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "MainWindowClass";
	return RegisterClassEx(&wc);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {
	MyRegisterClass(hInstance);
	
	MSG msg;

	HWND hwnd = CreateWindow(
		"MainWindowClass", APPTITLE.c_str(),
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
		//WS_EX_TOPMOST | WS_POPUP,0,0, //使得该窗口出现在所有其他窗口之上
		SCREENW, SCREENH, // 分辨率的设置640*480最小
		(HWND)NULL,
		(HMENU)NULL, hInstance, (LPVOID)NULL
		);
	if (hwnd == 0) return 0;
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	if (!Game_Init(hwnd)) return 0;

	while (!gameover) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Game_Run(hwnd);
		}
	}
	Game_End(hwnd);
	return msg.wParam;
}













