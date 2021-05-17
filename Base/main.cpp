
#include "base.h"

IDirect3DDevice9* Device = 0; 
D3DXMATRIX proj;
D3DXMATRIX view;

int Width  = 800;
int Height = 600;



bool Setup()
{


	// basic setup ////////////

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// set alpha blending stuff
	//
	Device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	Device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	Device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);

	// use alpha in material's diffuse component for alpha
	Device->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// set blending factors so that alpha component determines transparency
	Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//
	// Set projection matrix
	//

	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//////// end basic setups //////////

	return true;
}

void Cleanup()
{

}

bool Display(float timeDelta)
{


	if( Device )
	{






		//
		// Render
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();



		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D("Base", hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	ShowCursor(true);
	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}

