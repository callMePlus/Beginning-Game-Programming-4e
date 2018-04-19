// Beginning Game Programming
// Chapter 5 - XInput Test Program
// Press A/B to start/stop vibration

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <xinput.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"xinput.lib")

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

const string APPTITLE = "XInput Test Program";
const int SCREENW = 640;
const int SCREENH = 480;
LPDIRECT3D9 d3d = NULL; 
LPDIRECT3DDEVICE9 d3ddev = NULL; 
bool gameover = false;

// Initializes XInput and any connected controllers
bool XInput_Init(int contNum = 0)
{
    XINPUT_CAPABILITIES caps;
    ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
    XInputGetCapabilities(contNum, XINPUT_FLAG_GAMEPAD, &caps);
    if (caps.Type != XINPUT_DEVTYPE_GAMEPAD) return false;

    return true;
}

// Causes the controller to vibrate
void XInput_Vibrate(int contNum = 0, int left = 65535, int right = 65535)
{
    XINPUT_VIBRATION vibration;
    ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
    vibration.wLeftMotorSpeed = left;
    vibration.wRightMotorSpeed = right; 
    XInputSetState( contNum, &vibration );
}

// Checks the state of the controller
void XInput_Update()
{
    XINPUT_STATE state;
    string message = "";

    for (int i=0; i< 4; i++ )
    {
        ZeroMemory( &state, sizeof(XINPUT_STATE) );
        message = "";

        //get the state of the controller 
        DWORD result = XInputGetState( i, &state );

        //is controller connected?
        if( result == 0 )
        { 
            if (state.Gamepad.bLeftTrigger)
                message = "Left Trigger";
            else if (state.Gamepad.bRightTrigger)
                message = "Right Trigger";
            else if (state.Gamepad.sThumbLX < -10000 || state.Gamepad.sThumbLX > 10000)
                message = "Left Thumb Stick";
            else if (state.Gamepad.sThumbRX < -10000 || state.Gamepad.sThumbRX > 10000)
                message = "Right Thumb Stick";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
                message = "DPAD Up";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
                message = "DPAD Down";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
                message = "DPAD Left";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
                message = "DPAD Right";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
                message = "Start Button";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
                message = "Left Thumb";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
                message = "Right Thumb";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
                message = "Left Shoulder";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
                message = "Right Shoulder";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
            {
                XInput_Vibrate(0, 65535, 65535);
                message = "A Button";
            }
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
            {
                XInput_Vibrate(0, 0, 0);
                message = "B Button";
            }
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
                message = "X Button";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
                message = "Y Button";
            else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
                gameover = true;

            //if an event happened, then announce it
            if (message.length() > 0)
                MessageBox(0, message.c_str(), "Controller", 0);

        }
        else {
            // controller is not connected 
        }
    }
}

bool Game_Init(HWND hwnd)
{
    //initialize Direct3D
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d == NULL)
    {
        MessageBox(hwnd, "Error initializing Direct3D", "Error", MB_OK);
        return false;
    }

    //set Direct3D presentation parameters
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = SCREENW;
    d3dpp.BackBufferHeight = SCREENH;
    d3dpp.hDeviceWindow = hwnd;

    //create Direct3D device
    d3d->CreateDevice( D3DADAPTER_DEFAULT, 
        D3DDEVTYPE_HAL, hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
        &d3dpp, &d3ddev);

    if (!d3ddev)
    {
        MessageBox(hwnd, "Error creating Direct3D device", "Error", MB_OK);
        return false;
    }

    //initialize XInput
    XInput_Init();
    
    return true;
}

void Game_Run(HWND hwnd)
{
    if (!d3ddev) return;

    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,150), 1.0f, 0);
    if (d3ddev->BeginScene())
    {
        //no rendering yet

        d3ddev->EndScene();
        d3ddev->Present(NULL, NULL, NULL, NULL);
    }

    if (KEY_DOWN(VK_ESCAPE))
        PostMessage(hwnd, WM_DESTROY, 0, 0);

    XInput_Update();
}

void Game_End(HWND hwnd)
{
    if (d3ddev) d3ddev->Release();
    if (d3d) d3d->Release();
}

LRESULT WINAPI WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            gameover = true;
            break;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //create the window class structure
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX); 
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.lpszMenuName = NULL;
    wc.hInstance = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = "MainWindowClass";

    if (!RegisterClassEx(&wc))
        return FALSE;

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

    Game_End(window);

    return message.wParam;
}

