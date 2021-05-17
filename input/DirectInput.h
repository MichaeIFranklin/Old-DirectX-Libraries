#ifndef directinput_h_
#define directinput_h_

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <d3dx9.h>
#include <dinput.h>
#include "base.h"

class Input
{
	enum InType { MOUSE, KEYBOARD, XBOX};

private:
	LPDIRECTINPUT8 DirectInput;

	UCHAR KeyState[256];
	DIMOUSESTATE MouseState;
	GUID joystickGUID;
	DIJOYSTATE joy_state;
	bool joystickin;
	
	//Size offset variables
	float XSizeOffset;
	float YSizeOffset;

	bool UpdateKeys();
	bool UpdateMouse();
	bool UpdateJoy();
public:
	inline bool GetJoyStickIn()
	{
		return joystickin;
	}
	LPDIRECTINPUTDEVICE8 Keyboard;
	LPDIRECTINPUTDEVICE8 Mouse;
	LPDIRECTINPUTDEVICE8 lpDIJS;
	Input();
	bool Setup();
	void Cleanup();
	bool GetKey(int key);
	bool GetButton(int button);
	float GetMouseWheel();
	bool GetJoyButton(int button);
	void Update ();
	void GetMousePos(float &x, float &y);
	void SetMousePos(float x, float y);
	void GetJoyStickMovement(D3DXVECTOR2 &leftjoy, D3DXVECTOR2 &rightjoy);
	void GetAbsoluteMousePosition(float& x, float& y);

	void CreateRayFrom2D(D3DXVECTOR3 &RayOrigin, D3DXVECTOR3 &RayDirection, float X, float Y, IDirect3DDevice9 *Device);
	void CreateRayFrom2D(D3DXVECTOR3 &RayOrigin, D3DXVECTOR3 &RayDirection, float X, float Y, D3DVIEWPORT9 ViewPort, D3DXMATRIX View, D3DXMATRIX Projection);
};

extern char joyname[80];
extern BOOL CALLBACK DI_Enum_Joysticks(LPCDIDEVICEINSTANCE lpddi, LPVOID guid_ptr);

#endif