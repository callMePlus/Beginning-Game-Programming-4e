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

// Direct3d functions
bool Direct3D_Init(HWND window, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
// 整个source画到dest的(x,y)上
void DrawSurface(LPDIRECT3DSURFACE9 dest,float x,float y,LPDIRECT3DSURFACE9 source);

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

