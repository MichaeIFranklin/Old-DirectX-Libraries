#ifndef Lighting_h_
#define Lighting_h_

#include<d3dx9.h>

extern int LightOrder;

class Lighting
{
private:
	D3DLIGHT9 ClassLight;
	D3DMATERIAL9 LightMaterial;
	int LightNumber;
	bool LightActivated;
public:
	static int AmountOn;
	inline void DestroyLight()
	{
		LightOrder -= 1;
		if(LightActivated)
		{
			AmountOn--;
		}
	}
	inline void EnableClassLight(bool Toggle, IDirect3DDevice9* Device)
	{
		LightActivated = Toggle;
		if(Toggle)
		{
			AmountOn++;
		}
		else
		{
			AmountOn--;
		}
		Device->LightEnable(LightNumber,LightActivated);
	};

	inline D3DXVECTOR3 GetPosition()
	{
		return ClassLight.Position;
	};
	inline D3DXVECTOR3 GetDirection()
	{
		return ClassLight.Direction;
	};

	inline void SetPosition(D3DXVECTOR3 Position)
	{
		ClassLight.Position = Position;
	};
	inline void SetDirection(D3DXVECTOR3 Direction)
	{
		if(ClassLight.Type == D3DLIGHT_DIRECTIONAL || ClassLight.Type == D3DLIGHT_SPOT)
		{
			ClassLight.Direction = Direction;
		}
	};
	
	inline void SetAmbeint(D3DXCOLOR Ambient)
	{
		ClassLight.Ambient = Ambient;
	};
	inline void setDiffuse(D3DXCOLOR Diffuse)
	{
		ClassLight.Diffuse = Diffuse;
	};
	inline void SetPhi(float Phi)
	{
		ClassLight.Phi = Phi;
	};
	inline void setTheta(float Theta)
	{
		ClassLight.Theta = Theta;
	};
	inline void SetAttenuation0(float Att0)
	{
		ClassLight.Attenuation0 = Att0;
	};
	inline void setAttennuation1(float Att1)
	{
		ClassLight.Attenuation1 = Att1;
	};
	inline void SetAttenuation2(float Att2)
	{
		ClassLight.Attenuation2 = Att2;
	};
	inline void SetLightMaterial(D3DXCOLOR Ambient, D3DXCOLOR Diffuse, D3DXCOLOR Specular, D3DXCOLOR Shininess, float Power)
	{
		LightMaterial.Ambient = Ambient;
		LightMaterial.Diffuse = Diffuse;
		LightMaterial.Specular = Specular;
		LightMaterial.Power = Power;
		LightMaterial.Emissive = Shininess;
	};

	void CreateDirectionalLight(D3DXVECTOR3* Direction, D3DXCOLOR* Color, IDirect3DDevice9* Device);
	void CreateDirectionalLight(D3DXVECTOR3 Direction,D3DXCOLOR Color,float AmbientMultiply,float SpecularMultiply, IDirect3DDevice9* Device);
	void CreatePointLight(D3DXVECTOR3 Position, D3DXCOLOR Color,float Range, IDirect3DDevice9* Device);
	void CreateSpotLight(D3DXVECTOR3 Position, D3DXVECTOR3 Direction, D3DXCOLOR Color, float Range, IDirect3DDevice9* Device, float Falloff = 1.0, float Phi = 0.9, float Theta = 0.4);
	void UpdateLight(D3DXVECTOR3 Position,D3DXVECTOR3 Direction, IDirect3DDevice9* Device);
	void UpdateLight(D3DXVECTOR3 DirectionOrPosition, IDirect3DDevice9* Device);
};

extern void SetupVertexFog(DWORD Color, DWORD Mode, bool UseRange, FLOAT Density, IDirect3DDevice9* Device);

#endif