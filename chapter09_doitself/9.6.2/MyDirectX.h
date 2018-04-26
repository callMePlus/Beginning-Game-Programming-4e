#pragma once

#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

// libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")

// program values
extern const string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameover;
// Direct3D object
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;

extern LPD3DXFONT font;

// 精灵渲染对象
extern LPD3DXSPRITE spriteobj;

//sprite structure
struct SPRITE
{
	float x, y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	float velx, vely;
	D3DCOLOR color;

	SPRITE()
	{
		frame = 0;
		columns = 1;
		width = height = 0;
		scaling = 1.0f;
		rotation = 0.0f;
		startframe = endframe = 0;
		direction = 1;
		starttime = delay = 0;
		velx = vely = 0.0f;
		color = D3DCOLOR_XRGB(255, 255, 255);
	}
};

// Direct3d functions
bool Direct3D_Init(HWND window, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);

// 初始化字体
void DirectFont_Init();

// 整个source画到dest的(x,y)上
void DrawSurface(LPDIRECT3DSURFACE9 dest,float x,float y,LPDIRECT3DSURFACE9 source);
D3DXVECTOR2 GetBitmapSize(string filename);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int frameh, int columns);
void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image,int x,int y,int width,int height,int frame = 0,int columns = 1,float rotation = 1.0f,float scaling = 1.0f,D3DCOLOR color = D3DCOLOR_XRGB(255,255,255));
void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);
double toRadians(double radians);
int Collision(SPRITE sprite1, SPRITE sprite2);
void Font_Transform(int x, int y, int width, int height, float rotation, float scaling, D3DCOLOR color, int ori_x, int ori_y);

// DirectInput objects,device and states
extern LPDIRECTINPUT dinput;
extern LPDIRECTINPUTDEVICEA dimouse;
extern LPDIRECTINPUTDEVICEA dikeyboard;
extern DIMOUSESTATE mouse_state;
extern char keys[256];

// DirectInput functions
bool DirectInput_Init(HWND window);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int key);

// Game functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();

