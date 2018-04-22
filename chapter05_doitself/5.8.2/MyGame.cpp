#include "MyDirectX.h"
const string APPTITLE = "Bomb Catcher Game";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3DSURFACE9 bomb_surf = NULL;
LPDIRECT3DSURFACE9 bomb_surf1 = NULL;
LPDIRECT3DSURFACE9 bucket_surf = NULL;

struct BOMB{
	float x, y;
	void reset(){
		x = (float)(rand() % (SCREENW - 128));
		y = 0;
	}
}; 
BOMB bomb;
BOMB bomb1;

struct BUCKET{
	float x, y;
}; 

BUCKET bucket;
int score = 0;
int vibrating = 0;

bool Game_Init(HWND window){
	Direct3D_Init(window,SCREENW,SCREENH,true);
	DirectInput_Init(window);
	bomb_surf = LoadSurface("bomb.bmp");
	if (!bomb_surf){
		MessageBox(window,"Error loading bomb","Error",0);
		return false;
	}
	bomb_surf1 = LoadSurface("bomb.bmp");
	if (!bomb_surf1){
		MessageBox(window, "Error loading bomb", "Error", 0);
		return false;
	}

	bucket_surf = LoadSurface("bucket.bmp");
	if (!bucket_surf){
		MessageBox(window, "Error loading bucket", "Error", 0);
		return false;
	}
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
	srand((UINT)time(NULL));
	bomb.reset();
	bomb1.reset();
	bucket.x = 500;
	bucket.y = 630;
	return true;
}

void Game_End(){
	if (bomb_surf){
		bomb_surf->Release();
		bomb_surf = NULL;
	}
	if (bucket_surf){
		bucket_surf->Release();
		bucket_surf = NULL;
	}
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}

void KeepInScreen(){
	if (bucket.x < 0)
		bucket.x = 0;
	if (bucket.x > SCREENW - 128)
		bucket.x = SCREENW - 128;
	if (bucket.y< 0)
		bucket.y = 0;
	if (bucket.y > SCREENH - 128)
		bucket.y = SCREENH - 128;
}

void MovedByKeyBoard(){
	const float move_pixel = 3.0f;
	if (Key_Down(DIK_W)){
		bucket.y -= move_pixel;
	}
	if (Key_Down(DIK_A)){
		bucket.x -= move_pixel;
	}
	if (Key_Down(DIK_S)){
		bucket.y += move_pixel;
	}
	if (Key_Down(DIK_D)){
		bucket.x += move_pixel;
	}
}

void CheckCollide(HWND window,struct BOMB * bomb){
	int cx = (int)(*bomb).x + 64;
	int cy = (int)(*bomb).y + 64;
	if (cx > bucket.x && cx < bucket.x + 128 &&
		cy > bucket.y && cy < bucket.y + 128){
		(*bomb).reset();
	}
}
void Game_Run(HWND window){
	if (!d3ddev) return;
	DirectInput_Update();
	bomb.y += 2.0f;
	bomb1.y += 1.0f;
	if (bomb.y > SCREENH){
		MessageBox(window,"Oh,no,the bomb exploded","YOU STINK",0);
		gameover = true;
	}
	if (bomb1.y > SCREENH){
		MessageBox(window, "Oh,no,the bomb exploded", "YOU STINK", 0);
		gameover = true;
	}

	// 根据键盘移动物体
	MovedByKeyBoard();

	//保持bucket在屏幕里
	KeepInScreen();

	// 碰撞检测
	CheckCollide(window,&bomb);
	CheckCollide(window,&bomb1);

	d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));

	if (d3ddev->BeginScene()){
		DrawSurface(backbuffer, bomb.x, bomb.y, bomb_surf);
		DrawSurface(backbuffer, bomb1.x, bomb1.y, bomb_surf1);
		DrawSurface(backbuffer, bucket.x, bucket.y, bucket_surf);

		d3ddev->EndScene();
		d3ddev->Present(NULL,NULL,NULL,NULL);
	}

	if (Key_Down(DIK_ESCAPE)){
		gameover = true;
	}

}