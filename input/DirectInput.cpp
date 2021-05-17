#include "DirectInput.h"
char joyname[80];

Input::Input()
{
	DirectInput = NULL;
	Keyboard = NULL;
	Mouse = NULL;
	lpDIJS = NULL;
}

bool Input::UpdateKeys()
{
	HRESULT hr;
	hr = Keyboard->GetDeviceState(256,(LPVOID)KeyState);
	if (hr == DIERR_INPUTLOST)
	{
		if (FAILED(Keyboard->Acquire()))
		{
			Cleanup();
			Setup();
		}
	}
	return true;
}

bool Input::UpdateJoy()
{
	if (joystickin)
	{
		lpDIJS->Poll(); // this is needed for joysticks only
		lpDIJS->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joy_state);
	}
	return true;
}

BOOL CALLBACK DI_Enum_Joysticks(LPCDIDEVICEINSTANCE lpddi, LPVOID guid_ptr)

{

	// this function enumerates the joysticks, but

	// stops at the first one and returns the

	// instance guid of it, so we can create it

	*(GUID*)guid_ptr = lpddi->guidInstance;

	// copy name into global

	strcpy(joyname, (char *)lpddi->tszProductName);

	// stop enumeration after one iteration

	return(DIENUM_STOP);

}

bool Input::UpdateMouse()
{
	HRESULT hr;
	hr = Mouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&MouseState);
	if (hr == DIERR_INPUTLOST)
	{
		if (FAILED(Mouse->Acquire()))
		{
			Cleanup();
			Setup();
		}
	}
	return true;
}

float Input::GetMouseWheel()
{
	return MouseState.lZ;
}

bool Input::Setup()
{
	joystickin = false;

	//check for device
	if(FAILED(DirectInput8Create(Main_Instance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&DirectInput,NULL)))
	{return 0;}

	// Direct Input 8 - J O Y S T I C K

	//// first find the GUID of your particular joystick

	DirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL,&DI_Enum_Joysticks, &joystickGUID, DIEDFL_ATTACHEDONLY);

	if (FAILED(DirectInput->CreateDevice(joystickGUID, &lpDIJS, NULL)))
	{
		joystickin = false;
	}
	else
	{
		joystickin = true;
	}
	if(joystickin == true)
	{
		if (FAILED(lpDIJS->SetCooperativeLevel(Main_Hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))

		{
			return(0);
		}

		if (FAILED(lpDIJS->SetDataFormat(&c_dfDIJoystick)))

		{

			return(0);

		}

		//// set the range of the joystick

		DIPROPRANGE joy_axis_range;

		//// first x axis

		joy_axis_range.lMin = -100;

		joy_axis_range.lMax = 100;

		joy_axis_range.diph.dwSize = sizeof(DIPROPRANGE);

		joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER);

		joy_axis_range.diph.dwObj = DIJOFS_X;

		joy_axis_range.diph.dwHow = DIPH_BYOFFSET;

		lpDIJS->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);

		//// now y-axis

		joy_axis_range.lMin = -100;

		joy_axis_range.lMax = 100;

		joy_axis_range.diph.dwSize = sizeof(DIPROPRANGE);

		joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER);

		joy_axis_range.diph.dwObj = DIJOFS_Y;

		joy_axis_range.diph.dwHow = DIPH_BYOFFSET;

		lpDIJS->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);

		//// and now the dead band

		DIPROPDWORD dead_band; // here's our property word

		dead_band.diph.dwSize = sizeof(dead_band);

		dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);

		dead_band.diph.dwObj  = DIJOFS_X;

		dead_band.diph.dwHow = DIPH_BYOFFSET;

		// 10% will be used on both sides of the range +/-

		dead_band.dwData = 1000;

		//// finally set the property

		lpDIJS->SetProperty(DIPROP_DEADZONE,&dead_band.diph);

		dead_band.diph.dwSize = sizeof(dead_band);

		dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);

		dead_band.diph.dwObj = DIJOFS_Y;

		dead_band.diph.dwHow = DIPH_BYOFFSET;

		// 10% will be used on both sides of the range +/-

		dead_band.dwData = 1000;

		// finally set the property

		lpDIJS->SetProperty(DIPROP_DEADZONE,&dead_band.diph);

		// acquire the joystick

		if (FAILED(lpDIJS->Acquire()))

		{

			return(0);

		}
	}



	// Direct Input 8 - K E Y B O A R D
	if(FAILED(DirectInput->CreateDevice(GUID_SysKeyboard,&Keyboard,NULL)))
	{return 0;}

	if(FAILED(Keyboard->SetCooperativeLevel(Main_Hwnd,DISCL_NONEXCLUSIVE|DISCL_BACKGROUND)))
	{return 0;}

	if(FAILED(Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{return 0;}

	if(FAILED(Keyboard->Acquire()))
	{return 0;}

	// Direct Input 8 - M O U S E 
	if(FAILED(DirectInput->CreateDevice(GUID_SysMouse,&Mouse,NULL)))
	{return 0;}

	if(FAILED(Mouse->SetCooperativeLevel(Main_Hwnd,DISCL_NONEXCLUSIVE|DISCL_BACKGROUND)))
	{return 0;}

	if(FAILED(Mouse->SetDataFormat(&c_dfDIMouse)))
	{return 0;}

	if(FAILED(Mouse->Acquire()))
	{return 0;}

	return true;		
}

void Input::Cleanup()
{
	if (Mouse)
	{
		Mouse->Unacquire();
		Mouse->Release();
	}
	if (Keyboard)
	{
		Keyboard->Unacquire();
		Keyboard->Release();
	}
	DirectInput->Release();
	if(joystickin == true)
	{
		lpDIJS->Unacquire();
		if (lpDIJS)
			lpDIJS->Release();
	}
}

bool Input::GetKey(int key)
{
	if (KeyState[key] & 0x80)
		return true;
	else if (!(KeyState[key] & 0x80))
		return false;
	return false;
}

bool Input::GetButton(int button)
{
	if (MouseState.rgbButtons[button] & 0x80)
		return true;
	return false;
}

bool Input::GetJoyButton(int button)
{
	if(joystickin)
	{
		if (joy_state.rgbButtons[button] & 0x80)
			return true;
	}	
	return false;
}

void Input::Update()
{
	UpdateKeys();
	UpdateMouse();
	UpdateJoy();

	// get offset from window size to client size
	RECT ClientSize;
	GetClientRect(Main_Hwnd,&ClientSize);
	XSizeOffset = ClientSize.right / (float)WinWidth;
	YSizeOffset = ClientSize.bottom / (float)WinHeight;
}

void Input::GetMousePos(float &x, float &y)
{
	POINT MousePos;
	// get mouse position
	GetCursorPos(&MousePos);
	MapWindowPoints(NULL,Main_Hwnd,&MousePos,1);
	// adjust coords to account for window to client size difference
	x = MousePos.x / XSizeOffset;
	y = MousePos.y / YSizeOffset;
}
void Input::SetMousePos(float x, float y)
{
	POINT MousePos;
	// adjust coords to account for window to client size difference
	MousePos.x = x * XSizeOffset;
	MousePos.y = y * YSizeOffset;
	// set mouse position
	MapWindowPoints(Main_Hwnd,NULL,&MousePos,1);
	SetCursorPos(MousePos.x,MousePos.y);
}

void Input::GetJoyStickMovement(D3DXVECTOR2 &leftjoy, D3DXVECTOR2 &rightjoy)
{
	leftjoy = D3DXVECTOR2(joy_state.lX,joy_state.lY);
	rightjoy = D3DXVECTOR2(joy_state.lRx,joy_state.lRy);
}

void Input::GetAbsoluteMousePosition(float& x, float& y)
{
	x = 0.33*(float)MouseState.lX;
	y = 0.33*(float)MouseState.lY;
}

void Input::CreateRayFrom2D(D3DXVECTOR3 &RayOrigin, D3DXVECTOR3 &RayDirection, float X, float Y, IDirect3DDevice9 *Device)
{
	D3DVIEWPORT9 ViewPort; // Holds the current viewPort
	D3DXMATRIX View; // Holds the view matrix
	D3DXMATRIX World; // Holds the world matrix
	D3DXMATRIX Projection; // holds the projection matrix
	Device->GetViewport(&ViewPort); // Get the current viewPort
	Device->GetTransform(D3DTS_VIEW, &View); // Get the view matrix
	D3DXMatrixIdentity(&World); // Clear the world matrix
	Device->GetTransform(D3DTS_PROJECTION, &Projection); // Get the projection matrix

	// create a near a far vector using X and Y coordinates
	// near vector (picking ray originPoint)
	RayOrigin = D3DXVECTOR3(X,Y,0);
	// far vector (picking ray EndPoint)
	RayDirection = D3DXVECTOR3(X,Y,1);

	D3DXVec3Unproject(&RayOrigin,&RayOrigin,&ViewPort,&Projection,&View,&World); // plot the near vector point in 3D space 
	D3DXVec3Unproject(&RayDirection, &RayDirection, &ViewPort,&Projection,&View,&World); // plot the far vector point in 3D space
	RayDirection -= RayOrigin; // make a direction vector from the new near and far vectors
	D3DXVec3Normalize(&RayDirection, &RayDirection); // normalize the new direction vector
}
void Input::CreateRayFrom2D(D3DXVECTOR3 &RayOrigin, D3DXVECTOR3 &RayDirection, float X, float Y, D3DVIEWPORT9 ViewPort, D3DXMATRIX View, D3DXMATRIX Projection)
{
	D3DXMATRIX World; // Holds the world matrix
	D3DXMatrixIdentity(&World); // Clear the world matrix

	//create a near a far vector using X and Y coordinates
	// near vector (picking ray originPoint)
	RayOrigin = D3DXVECTOR3(X,Y,0);
	// far vector (picking ray EndPoint)
	RayDirection = D3DXVECTOR3(X,Y,1);
	
	D3DXVec3Unproject(&RayOrigin,&RayOrigin,&ViewPort,&Projection,&View,&World); // plot the near vector point in 3D space 
	D3DXVec3Unproject(&RayDirection, &RayDirection, &ViewPort,&Projection,&View,&World); // plot the far vector point in 3D space
	RayDirection -= RayOrigin; // make a direction vector from the new near and far vectors
	D3DXVec3Normalize(&RayDirection, &RayDirection); // normalize the new direction vector
}