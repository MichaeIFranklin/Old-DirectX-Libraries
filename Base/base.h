#ifndef __baseH__
#define __baseH__
#define WIN32_LEAN_AND_MEAN
#include <d3dx9.h>
#include <string>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

// Externals
extern HWND Main_Hwnd;
extern HINSTANCE Main_Instance;
extern int WinWidth;
extern int WinHeight;

namespace d3d
{
	bool InitD3D(char Title[], 
		HINSTANCE hInstance, // [in] Application instance.
		int width, int height, // [in] Backbuffer dimensions.
		bool windowed, 
		D3DDEVTYPE deviceType, // [in] HAL or REF
		IDirect3DDevice9** device);// [out]The created device.
	int EnterMsgLoop( 
		bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);
	template<class T> void Release(T t)
	{
		if( t )
		{
			t->Release();
			t = 0;
		}
	}
	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}
}
#endif // __baseH__
