#include "Lighting.h"

int LightOrder;
int Lighting::AmountOn = 0;

void Lighting::CreateDirectionalLight(D3DXVECTOR3* Direction,D3DXCOLOR* Color, IDirect3DDevice9* Device)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light,sizeof(Light));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Ambient = *Color * 0.6f;
	Light.Diffuse = *Color;
	Light.Specular = *Color * 0.8f;
	Light.Direction = *Direction;

	ClassLight = Light;
	LightOrder += 1;
	LightNumber = LightOrder;
	Device->SetLight(LightNumber,&ClassLight);
}

void Lighting::CreateDirectionalLight( D3DXVECTOR3 Direction,D3DXCOLOR Color,float AmbientMultiply,float SpecularMultiply, IDirect3DDevice9* Device )
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light,sizeof(Light));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Ambient = Color * AmbientMultiply;
	Light.Diffuse = Color;
	Light.Specular = Color * SpecularMultiply;
	Light.Direction = Direction;

	ClassLight = Light;
	LightOrder += 1;
	LightNumber = LightOrder;
	Device->SetLight(LightNumber,&ClassLight);
}



void Lighting::CreateSpotLight(D3DXVECTOR3 Position, D3DXVECTOR3 Direction, D3DXCOLOR Color, float Range, IDirect3DDevice9* Device, float Falloff, float Phi, float Theta)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(Light));

	Light.Type = D3DLIGHT_SPOT;
	Light.Ambient = Color * 0.0f;
	Light.Diffuse = Color;
	Light.Specular = Color * 0.8f;
	Light.Position = Position;
	Light.Range = Range;
	Light.Falloff = Falloff;
	Light.Direction = Direction;
	Light.Attenuation0 = 1.0f;
	Light.Theta = Theta;
	Light.Phi = Phi;

	ClassLight = Light;
	LightOrder += 1;
	LightNumber = LightOrder;
	Device->SetLight(LightNumber,&ClassLight);
}

void Lighting::CreatePointLight(D3DXVECTOR3 Position, D3DXCOLOR Color, float Range, IDirect3DDevice9* Device)
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light,sizeof(Light));

	Light.Type = D3DLIGHT_POINT;
	Light.Ambient = Color * 0.6f;
	Light.Diffuse = Color;
	Light.Specular = Color * 0.8f;
	Light.Position = Position;
	Light.Range = Range;
	Light.Attenuation0 = 0.0f;
	Light.Attenuation1 = (((100/Range)*100) * .115)/100.0;
	Light.Attenuation2 = 0.0f;

	
	ClassLight = Light;
	LightOrder += 1;
	LightNumber = LightOrder;
	Device->SetLight(LightNumber,&ClassLight);
}

void Lighting::UpdateLight(D3DXVECTOR3 Position, D3DXVECTOR3 Direction, IDirect3DDevice9* Device)
{
	ClassLight.Position = Position;
	ClassLight.Direction = Direction;
	Device->SetLight(LightNumber,&ClassLight);
}

void Lighting::UpdateLight(D3DXVECTOR3 DirectionOrPosition, IDirect3DDevice9* Device)
{
	if(ClassLight.Type == D3DLIGHT_DIRECTIONAL)
	{
		ClassLight.Direction = DirectionOrPosition;
	}
	else
	{
		ClassLight.Position = DirectionOrPosition;
	}
	Device->SetLight(LightNumber,&ClassLight);
}


void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density,  IDirect3DDevice9* Device)
{
	float Start = 0.5f,    // Linear fog distances
		End   = 0.8f;

	// Enable fog blending.
	Device->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// Set the fog color.
	Device->SetRenderState(D3DRS_FOGCOLOR, Color);

	// Set fog parameters.
	if(D3DFOG_LINEAR == Mode)
	{
		Device->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		Device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
		Device->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
	}
	else
	{
		Device->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		Device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
	}

	if(UseRange)
		Device->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
}

