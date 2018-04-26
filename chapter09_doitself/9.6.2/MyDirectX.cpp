#include "MyDirectX.h"
using namespace std;

const double PI = 3.1415926;
const double PI_over_180 = PI / 180.0f;

// Direct3D variables
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPD3DXSPRITE spriteobj;

LPD3DXFONT font = NULL;

// Direct3DInput
LPDIRECTINPUT dinput = NULL;
LPDIRECTINPUTDEVICEA dimouse = NULL;
LPDIRECTINPUTDEVICEA dikeyboard = NULL;
DIMOUSESTATE mouse_state;
char keys[256];

bool Direct3D_Init(HWND window,int width,int height,bool fullscreen){
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) return false;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	d3dpp.Windowed = fullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferWidth = width;
	d3dpp.hDeviceWindow = window;
	d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,window,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&d3ddev);
	if (!d3ddev) return false;
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
	
	// 初始化精灵
	HRESULT result =  D3DXCreateSprite(d3ddev,&spriteobj);
	if (result != S_OK) return false;
	return true;
}

void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int frameh, int columns)
{
	D3DXVECTOR3 position((float)destx, (float)desty, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;
	rect.left = (framenum % columns) * framew;
	rect.top = (framenum / columns) * frameh;
	rect.right = rect.left + framew;
	rect.bottom = rect.top + frameh;

	spriteobj->Draw(texture, &rect, NULL, &position, white);
}
void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height, int frame, int columns, float rotation, float scaling, D3DCOLOR color){
	// 控制缩放比例
	D3DXVECTOR2 scale(scaling, scaling);
	// 控制平移
	D3DXVECTOR2 trans((float)x, (float)y);
	// 旋转中心
	D3DXVECTOR2 center((float)(width * scaling) / 2, (float)(height*scaling) / 2);
	D3DXVECTOR2 scenter((float)width / 2, (float)height / 2);

	D3DXMATRIX mat;
	
	D3DXMatrixTransformation2D(
		&mat, 
		&center,				/*缩放中心*/
		0,
		&scale					/*缩放*/, 
		&center				/*旋转中心*/,
		rotation				/*旋转度数*/,
		&trans					/*平移*/);
	spriteobj->SetTransform(&mat);
	
	//cout << mat << endl;
	int fx = (frame % columns)* width;
	int fy = (frame / columns)* height;
	RECT srcRect = { fx, fy, fx + width, fy + height };

	spriteobj->Draw(image, &srcRect, NULL, NULL, color);
}

void Font_Transform(int x, int y, int width, int height, float rotation, float scaling, D3DCOLOR color,int ori_x,int ori_y){
	// 控制缩放比例
	D3DXVECTOR2 scale(scaling, scaling);
	// 控制平移
	D3DXVECTOR2 trans((float)x, (float)y);
	// 旋转中心
	D3DXVECTOR2 scenter((float)width / 2 + ori_x, (float)(height / 2) + ori_y);
	D3DXVECTOR2 center((float)(width * scaling) / 2, (float)(height*scaling) / 2);

	D3DXMATRIX mat;

	D3DXMatrixTransformation2D(
		&mat,
		&scenter,				/*缩放中心*/
		0,
		&scale					/*缩放*/,
		&scenter				/*旋转中心*/,
		rotation				/*旋转度数*/,
		&trans					/*平移*/);
	spriteobj->SetTransform(&mat);
}

double toRadians(double degrees){
	return degrees * PI_over_180;
}

int Collision(SPRITE sprite1, SPRITE sprite2){
	RECT rect1;
	rect1.left = (long)sprite1.x;
	rect1.top = (long)sprite1.y;
	rect1.right = (long)(sprite1.x + sprite1.width * sprite1.scaling);
	rect1.bottom = (long)(sprite1.y + sprite1.height * sprite1.scaling);
	
	RECT rect2;
	rect2.left = (long)sprite2.x;
	rect2.top = (long)sprite2.y;
	rect2.right = (long)(sprite2.x + sprite2.width * sprite2.scaling);
	rect2.bottom = (long)(sprite2.y + sprite2.height * sprite2.scaling);

	RECT dest; //ignored
	return IntersectRect(&dest, &rect1, &rect2);
}

void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay)
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe) frame = startframe;
		if (frame < startframe) frame = endframe;
	}
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
	if (spriteobj){
		spriteobj->Release();
		spriteobj = NULL;
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


D3DXVECTOR2 GetBitmapSize(string filename){
	D3DXIMAGE_INFO image_info;
	if (D3D_OK != D3DXGetImageInfoFromFile(filename.c_str(), &image_info)) 
		return D3DXVECTOR2(0.0f,0.0f);
	return D3DXVECTOR2((float)image_info.Width, (float)image_info.Height);
}

LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor){
	LPDIRECT3DTEXTURE9 texture = NULL;
	D3DXIMAGE_INFO image_info;
	HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(),&image_info);
	if (result != D3D_OK) return NULL;
	result = D3DXCreateTextureFromFileEx(
		d3ddev,
		filename.c_str(),
		image_info.Width,
		image_info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transcolor,
		&image_info,
		NULL,
		&texture
		);
	if (result != D3D_OK) return NULL;
	return texture;
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

void DirectFont_Init(){
	D3DXFONT_DESC desc = {
		24,
		0,
		0,
		0,
		false,
		DEFAULT_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_PITCH,
		"Arial"
	};
	D3DXCreateFontIndirect(d3ddev,&desc,&font);

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