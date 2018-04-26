#include "MyDirectX.h"
const string APPTITLE = "Font Demo";
const int SCREENW = 1024;
const int SCREENH = 768;

D3DCOLOR color;

bool Game_Init(HWND window){
	Direct3D_Init(window,SCREENW,SCREENH,true);

	DirectInput_Init(window);

	DirectFont_Init();

	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
	srand((UINT)time(NULL));

	return true;
}

void Game_End(){
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
	static float tran = 0.1f;
	// rotate
	r = timeGetTime() / 600.0f;
	// scaling
	s += scale;
	if (s < 0.1f || s > 1.25f)
		scale *= -1;
	color = D3DCOLOR_XRGB(255, 255, 255);
	tran_x += tran;
	if (tran_x < 0 || tran_x>SCREENW - 500){
		tran *= -1;
	}

	DirectInput_Update();

	RECT rect;
	rect = { 100,200, 500, 300 };
	color = D3DCOLOR_XRGB(255, 255, 255);
	string text = "我皮一下挺开心1，我皮一下挺开心2，我皮一下挺开心3，我皮一下挺开心4，我皮一下挺开心5，我皮一下挺开心6，我皮一下挺开心7。";

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (d3ddev->BeginScene()){

		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		Font_Transform(tran_x, tran_y, rect.right-rect.left, rect.bottom-rect.top, r, s, color, rect.left, rect.top);
		font->DrawTextA(spriteobj, text.c_str(), text.length(), &rect, DT_WORDBREAK, color);

		spriteobj->End();
		//Sleep(10);
		d3ddev->EndScene();
		d3ddev->Present(NULL,NULL,NULL,NULL);
	}

	if (Key_Down(DIK_ESCAPE)){
		gameover = true;
	}

}