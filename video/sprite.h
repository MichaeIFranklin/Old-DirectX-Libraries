// Displays static and animated sprites (animated sprites not fully implemented yet)
// By Michael Franklin

#ifndef sprite_h__
#define sprite_h__

#include <d3dx9.h>
#include <Windows.h>
#include <stdio.h>

class sprite
{
private:
	IDirect3DDevice9* Device;
	LPD3DXSPRITE Sprite;
	LPDIRECT3DTEXTURE9 Texture;
	D3DXVECTOR2 Size;
	D3DXVECTOR2 Position;	
	D3DXVECTOR2 Center;
	D3DXVECTOR2 Scale;
	DWORD spriteColor;
	float Rotation;
	float fCurrentSpeed;
	//matrix
	D3DXMATRIX mTrans;

public:
	// these inline functions should be self explaining, their names say what they do
	// "getPosition" gets the position of the sprite
	sprite()
	{
		Sprite = NULL;
		Texture = NULL;
	}
	~sprite()
	{
		
		if (Sprite)
		{
			Sprite->Release();
			Sprite = 0;
		}
		if (Texture)
		{
			Texture = 0;
			delete Texture;
		}
		Device = 0;
		delete Device;
	}

	inline D3DXVECTOR2 getPosition()
	{
		return Position;
	};
	inline LPDIRECT3DTEXTURE9 getTexture()
	{
		return Texture;
	}
	inline float getRotation(bool inDegrees)
	{
		if (inDegrees)
		{
			return (Rotation*(D3DXToRadian(180)));
		}
		else
			return Rotation;
	};
	inline D3DXVECTOR2 getScale()
	{
		return Scale;	
	};
	inline void ChangeSize(D3DXVECTOR2 size)
	{
		Size = size;
		D3DSURFACE_DESC Desc;
		Texture->GetLevelDesc(0, &Desc);
		if (Size == D3DXVECTOR2(0, 0))
		{
			Size.x = Desc.Width;
			Size.y = Desc.Height;
		}
		Scale.x = Size.x / Desc.Width;
		Scale.y = Size.y / Desc.Height;
	}
	inline D3DXVECTOR2 getSize()
	{
		return Size;	
	};
	inline void setScale(float x, float y)
	{
		Scale = D3DXVECTOR2(x,y);
		Size.x *= Scale.x;
		Size.y *= Scale.y;
	};
	inline void ChangeCenter(float x, float y)
	{
		Center = D3DXVECTOR2(x,y);
	};
	inline DWORD getColor()
	{
		return spriteColor;
	};

	void ChangeTexture(LPDIRECT3DTEXTURE9 Texture);

	void Create(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 Texture,
		D3DXVECTOR2 size = D3DXVECTOR2(0, 0), D3DXVECTOR2 position = D3DXVECTOR2(0, 0), 
		D3DXVECTOR2 scale = D3DXVECTOR2(0, 0), D3DCOLOR initSpriteColor = D3DXCOLOR(255, 255, 255, 255), float rotation = 0.0f,
		D3DXVECTOR2 center = D3DXVECTOR2(0, 0));

	void Rotate(float rotation);
	void Move(D3DXVECTOR2 vector);

	// call this when you want to change the sprites color. pass it a d3dxcolor
	void ChangeColor(D3DCOLOR color);

	// call this in between the start scene and end scene in display. draw the sprite
	void Draw();
};
#endif
