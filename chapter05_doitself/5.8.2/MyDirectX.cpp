#include "MyDirectX.h"
using namespace std;
// Direct3D variables
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
// Direct3DInput
LPDIRECTINPUT dinput = NULL;
LPDIRECTINPUTDEVICE dimouse = NULL;
LPDIRECTINPUTDEVICE dikeyboard = NULL;
DIMOUSESTATE mouse_state;
char keys[256];

bool Direct3D_Init(HWND window,int width,int height,bool fullscreen){
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) return false;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = fullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferWidth = width;
	d3dpp.hDeviceWindow = window;
	d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,window,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&d3ddev);
	if (!d3ddev) return false;
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
	return true;
}

void Direct3D_Shutdown(){
	if (d3ddev){
		d3ddev->Release();
		d3ddev = NULL;
	}
	if (d3d){
		d3d->Release();
		d3d = NULL;
	}
}

void DrawSurface(LPDIRECT3DSURFACE9 dest,float x,float y,LPDIRECT3DSURFACE9 source){
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);
	RECT source_rect = { 0, 0, (long)desc.Width, (long)desc.Height };
	RECT dest_rect = { (long)x, (long)y, (long)x + desc.Width, (long)y + desc.Height };
	d3ddev->StretchRect(source, &source_rect,dest,&dest_rect,D3DTEXF_NONE);
}

LPDIRECT3DSURFACE9 LoadSurface(string filename){
	LPDIRECT3DSURFACE9 image = NULL;
	D3DXIMAGE_INFO image_info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(),&image_info);
	if (result != D3D_OK) return NULL;
	
	d3ddev->CreateOffscreenPlainSurface(
		image_info.Width,
		image_info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL
		);
	if (result != D3D_OK) return NULL;
	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		filename.c_str(),
		NULL,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0,0,0),
		NULL
		);
	if (result != D3D_OK) return NULL;
	return image;
}

bool DirectInput_Init(HWND window){
	ZeroMemory(keys, sizeof(keys));
	// 初始化DirectInput
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void **)&dinput,
		NULL
		);
	if (result != DI_OK) return false;

	// 创建DirectInput的keyboard和mouse对象
	result = dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
	if (result != D3D_OK) return false;
	result = dikeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (result != DI_OK) return false;
	result = dikeyboard->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (result != DI_OK) return false;
	result = dikeyboard->Acquire();
	if (result != DI_OK) return false;

	result = dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
	if (result != D3D_OK) return false;
	result = dimouse->SetDataFormat(&c_dfDIMouse);
	if (result != DI_OK) return false;
	result = dimouse->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (result != DI_OK) return false;
	result = dimouse->Acquire();
	if (result != DI_OK) return false;

	d3ddev->ShowCursor(false);
	return true;
}

void DirectInput_Update(){
	dimouse->GetDeviceState(sizeof(mouse_state),&mouse_state);
	dikeyboard->GetDeviceState(sizeof(keys),&keys);
}
int Key_Down(int key){
	return keys[key] & 0x80;
}


void DirectInput_Shutdown(){
	if (dikeyboard){
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}
	if (dimouse){
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
}