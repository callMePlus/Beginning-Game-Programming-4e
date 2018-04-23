#include "MyDirectX.h"
const string APPTITLE = "Transparent Sprite Demo";
const int SCREENW = 1024;
const int SCREENH = 768;

const int SPRITE_NUM = 30;
LPDIRECT3DTEXTURE9 image[SPRITE_NUM];
int frame[SPRITE_NUM];
int rand_show[SPRITE_NUM];
int starttime = 0;
int show_x = 0;
int show_y = 0;
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

bool Game_Init(HWND window){
	Direct3D_Init(window,SCREENW,SCREENH,true);
	DirectInput_Init(window);
	// 未设置颜色键值，默认键值是黑色
	for (size_t i = 0; i < SPRITE_NUM; i++){
		image[i] = LoadTexture("explosion_30_128.tga");
		if (!image[i]) return false;
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
	if (image){
		for (size_t i = 0; i < SPRITE_NUM; i++){
			image[i]->Release();
			image[i] = NULL;
		}
	}
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}

void Game_Run(HWND window){
	if (!d3ddev) return;

	DirectInput_Update();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (d3ddev->BeginScene()){

		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		for (size_t i = 0; i < SPRITE_NUM; i++){
			rand_show[i] = rand() % 300;
			Sprite_Animate(frame[i], 0, 29, 1, starttime, rand_show[i]);
		}
		show_y = 0;
		for (size_t i = 0; i < SPRITE_NUM; i++){
			show_x = 128 * (i % (SCREENW / 128));
			show_y = 128 * (i / (SCREENW / 128));
			Sprite_Draw_Frame(image[i], show_x, show_y, frame[i], 128, 128, 6);
		}
		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL,NULL,NULL,NULL);
	}

	if (Key_Down(DIK_ESCAPE)){
		gameover = true;
	}

}