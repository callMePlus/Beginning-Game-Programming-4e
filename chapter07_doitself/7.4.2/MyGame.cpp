#include "MyDirectX.h"
const string APPTITLE = "Sprite Rotation and Scaling Demo";
const int SCREENW = 1024;
const int SCREENH = 768;

LPDIRECT3DTEXTURE9 test;
LPDIRECT3DTEXTURE9 sunflower;
D3DCOLOR color;
int frame = 0, columns = 0, width = 0, height = 0;
int startframe = 0, endframe = 0, starttime = 0, delay = 0;

bool Game_Init(HWND window){
	Direct3D_Init(window,SCREENW,SCREENH,true);
	DirectInput_Init(window);
	// 未设置颜色键值，默认键值是黑色
	sunflower = LoadTexture("sunflower.bmp");
	test = LoadTexture("paladin_walk.png");

	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
	srand((UINT)time(NULL));

	return true;
}

void Game_End(){
	if (sunflower){
		sunflower->Release();
		sunflower = NULL;
	}
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}

void Game_Run(HWND window){
	if (!d3ddev) return;

	static float scale = 0.001f;
	static float r = 0;
	static float s = 1.0f;
	static float tran_x = 1.0f;
	static float tran_y = 1.0f;

	DirectInput_Update();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (d3ddev->BeginScene()){

		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		//spriteobj->OnResetDevice();
		// rotate
		r = timeGetTime() / 600.0f;
		// scaling
		s += scale;
		if (s < 0.1f || s > 1.25f)
			scale *= -1;
		width = height = 512;
		frame = 0;
		columns = 1;
		color = D3DCOLOR_XRGB(255,255,255);
		
		Sprite_Transform_Draw(sunflower, 
			SCREENW / 2 - width / 2,SCREENH / 2 - height / 2,
			width, height, 
			frame, columns, r, s, color);

		spriteobj->End();

		d3ddev->EndScene();
		d3ddev->Present(NULL,NULL,NULL,NULL);
	}

	if (Key_Down(DIK_ESCAPE)){
		gameover = true;
	}

}