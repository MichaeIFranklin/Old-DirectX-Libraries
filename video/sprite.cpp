// The cpp for my Sprite Class
// By Michael Franklin

#include "sprite.h"

void sprite::Create(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 Texture, D3DXVECTOR2 size, D3DXVECTOR2 position, 
	D3DXVECTOR2 scale, D3DCOLOR initSpriteColor, float rotation, D3DXVECTOR2 center)
{
	this->Device = Device;
	Sprite = NULL;
	this->Texture = NULL;
	Size = size;
	Position = position;
	Center = center;
	spriteColor = initSpriteColor;
	Scale = scale;
	Rotation = rotation;
	D3DXCreateSprite(Device, &Sprite);
	if (Texture != NULL)
	{
		this->Texture = Texture;
		ChangeSize(Size);
	}
}

void sprite::ChangeTexture(LPDIRECT3DTEXTURE9 Texture)
{
	if (Texture != NULL)
	{
		this->Texture = Texture;
		ChangeSize(Size);
	}
	else
	{
		this->Texture = NULL;
	}
}

void sprite::Rotate(float rotation)
{
	Rotation = rotation;
}

void sprite::Move(D3DXVECTOR2 vector)
{
	Position = vector;
}

void sprite::ChangeColor(DWORD color)
{
	spriteColor = color;
}

void sprite::Draw()
{
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	if (Texture != NULL)
	{
		D3DXVECTOR2 NewPosition = Position - Center;	
		D3DXMatrixTransformation2D(&mTrans, &Center, 0.0f, &Scale, &Center, Rotation, &NewPosition);
		Sprite->SetTransform(&mTrans);
		if(S_OK != Sprite->Draw(Texture, NULL, NULL,NULL, spriteColor))
			::MessageBox(NULL, "Failed to draw sprite", "oops", MB_OK);
	}
	
	Sprite->End();
}