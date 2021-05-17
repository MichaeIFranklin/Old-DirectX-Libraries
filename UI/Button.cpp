// The cpp for my Button class
// By Michael Franklin

#include "Button.h"

void Button::Create(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 IdleTexture, LPDIRECT3DTEXTURE9 HoverTexture, LPDIRECT3DTEXTURE9 ClickedTexture, int ButtonSpriteWidth, int ButtonSpriteHeight, float PositionX, float PositionY, D3DXVECTOR2 Scale)
{
	this->Device = Device;
	this->PositionX = PositionX;
	this->PositionY = PositionY;
	this->Scale = Scale;
	this->ButtonSpriteHeight = ButtonSpriteHeight;
	this->ButtonSpriteWidth = ButtonSpriteWidth;
	this->IdleSprite.Create(Device,IdleTexture,D3DXVECTOR2(ButtonSpriteWidth*Scale.x,ButtonSpriteHeight*Scale.y),D3DXVECTOR2(PositionX,PositionY));
	this->HoverSprite.Create(Device,HoverTexture,D3DXVECTOR2(ButtonSpriteWidth*Scale.x,ButtonSpriteHeight*Scale.y),D3DXVECTOR2(PositionX,PositionY));
	this->ClickedSprite.Create(Device,ClickedTexture,D3DXVECTOR2(ButtonSpriteWidth*Scale.x,ButtonSpriteHeight*Scale.y),D3DXVECTOR2(PositionX,PositionY));
	this->ButtonStage = 0;
	this->ButtonType = 0;
}
void Button::Create(LPDIRECT3DDEVICE9 Device, int IdleFontSize, int IdleFontWeight, bool IdleFontItalic,char IdleFontName[], DWORD IdleFontColor, char IdleFontText[], int HoverFontSize, int HoverFontWeight, bool HoverFontItalic,char HoverFontName[], DWORD HoverFontColor, char HoverFontText[], int ClickedFontSize, int ClickedFontWeight, bool ClickedFontItalic,char ClickedFontName[], DWORD ClickedFontColor, char ClickedFontText[], float PositionX, float PositionY, float Scale)
{
	this->Device = Device;
	this->PositionX = PositionX;
	this->PositionY = PositionY;
	this->Scale = D3DXVECTOR2(Scale,Scale);
	this->IdleFontText = IdleFontText;
	this->HoverFontText = HoverFontText;
	this->ClickedFontText = ClickedFontText;
	this->IdleFont.SetupFont(Device,IdleFontSize*Scale,IdleFontWeight,IdleFontItalic,IdleFontName,IdleFontColor,PositionX,PositionY,1);
	this->HoverFont.SetupFont(Device,HoverFontSize*Scale,HoverFontWeight,HoverFontItalic,HoverFontName,HoverFontColor,PositionX,PositionY,1);
	this->ClickedFont.SetupFont(Device,ClickedFontSize*Scale,ClickedFontWeight,ClickedFontItalic,ClickedFontName,ClickedFontColor,PositionX,PositionY,1);
	this->ButtonSpriteHeight = this->IdleFont.GetRectDimensions(this->IdleFontText).y;
	this->ButtonSpriteWidth = this->IdleFont.GetRectDimensions(this->IdleFontText).x;
	this->ButtonStage = 0;
	this->ButtonType = 1;
}
void Button::Create(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 IdleTexture, LPDIRECT3DTEXTURE9 HoverTexture, LPDIRECT3DTEXTURE9 ClickedTexture, int IdleFontWeight, bool IdleFontItalic,char IdleFontName[], DWORD IdleFontColor, char IdleFontText[], int HoverFontWeight, bool HoverFontItalic,char HoverFontName[], DWORD HoverFontColor, char HoverFontText[], int ClickedFontWeight, bool ClickedFontItalic,char ClickedFontName[], DWORD ClickedFontColor, char ClickedFontText[], int ButtonSpriteWidth, int ButtonSpriteHeight, float PositionX, float PositionY, D3DXVECTOR2 Scale)
{
	this->Device = Device;
	this->PositionX = PositionX;
	this->PositionY = PositionY;
	this->Scale = Scale;
	this->ButtonSpriteHeight = ButtonSpriteHeight;
	this->ButtonSpriteWidth = ButtonSpriteWidth;
	this->IdleSprite.Create(Device,IdleTexture,D3DXVECTOR2(ButtonSpriteWidth*Scale.x,ButtonSpriteHeight*Scale.y),D3DXVECTOR2(PositionX,PositionY));
	this->HoverSprite.Create(Device,HoverTexture,D3DXVECTOR2(ButtonSpriteWidth*Scale.x,ButtonSpriteHeight*Scale.y),D3DXVECTOR2(PositionX,PositionY));
	this->ClickedSprite.Create(Device,ClickedTexture,D3DXVECTOR2(ButtonSpriteWidth*Scale.x,ButtonSpriteHeight*Scale.y),D3DXVECTOR2(PositionX,PositionY));
	this->IdleFontText = IdleFontText;
	this->HoverFontText = HoverFontText;
	this->ClickedFontText = ClickedFontText;
	D3DXVECTOR2 TempVec;
	this->IdleFont.SetupFont(Device,(40 * ButtonSpriteWidth)/200.0,IdleFontWeight,IdleFontItalic,IdleFontName,IdleFontColor,PositionX,PositionY,Scale.x);
	TempVec = this->IdleFont.GetRectDimensions(this->IdleFontText);
	while(TempVec.x > (ButtonSpriteWidth/1.1) * Scale.x || TempVec.y > ButtonSpriteHeight * Scale.y)
	{
		this->IdleFont.ChangeSize(this->IdleFont.GetSize()-1);
		TempVec = this->IdleFont.GetRectDimensions(this->IdleFontText);
	};
	this->IdleFont.ChangePosition((PositionX+(ButtonSpriteWidth*Scale.x/2))-(this->IdleFont.GetRectDimensions(IdleFontText).x/2),(PositionY+(ButtonSpriteHeight*Scale.y/2))-(this->IdleFont.GetRectDimensions(IdleFontText).y/2));
	this->HoverFont.SetupFont(Device,(40 * ButtonSpriteWidth)/200.0,HoverFontWeight,HoverFontItalic,HoverFontName,HoverFontColor,PositionX,PositionY,Scale.x);
	TempVec = this->HoverFont.GetRectDimensions(this->HoverFontText);
	while(TempVec.x > (ButtonSpriteWidth/1.1) * Scale.x || TempVec.y > ButtonSpriteHeight * Scale.y)
	{
		this->HoverFont.ChangeSize(this->HoverFont.GetSize()-1);
		TempVec = this->HoverFont.GetRectDimensions(this->HoverFontText);
	};
	this->HoverFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).y/2));
	this->ClickedFont.SetupFont(Device,(40 * ButtonSpriteWidth)/200.0,ClickedFontWeight,ClickedFontItalic,ClickedFontName,ClickedFontColor,PositionX,PositionY,Scale.x);
	TempVec = this->ClickedFont.GetRectDimensions(this->ClickedFontText);
	while(TempVec.x > (ButtonSpriteWidth/1.1) * Scale.x || TempVec.y > ButtonSpriteHeight * Scale.y)
	{
		this->ClickedFont.ChangeSize(this->ClickedFont.GetSize()-1);
		TempVec = this->ClickedFont.GetRectDimensions(this->ClickedFontText);
	};
	this->ClickedFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).y/2));
	this->ButtonStage = 0;
	this->ButtonType = 2;
}

void Button::ChangeSprite(LPDIRECT3DTEXTURE9 IdleTexture, LPDIRECT3DTEXTURE9 HoverTexture, LPDIRECT3DTEXTURE9 ClickedTexture)
{
	this->IdleSprite.ChangeTexture(IdleTexture);
	this->HoverSprite.ChangeTexture(HoverTexture);
	this->ClickedSprite.ChangeTexture(ClickedTexture);
}

void Button::ChangeScale(D3DXVECTOR2 Scale)
{
	switch(this->ButtonType)
	{
	case 0:
		this->Scale = Scale;
		if (IdleSprite.getTexture() != NULL)
			this->IdleSprite.ChangeSize(D3DXVECTOR2(ButtonSpriteWidth * Scale.x,ButtonSpriteHeight * Scale.y));
		if (HoverSprite.getTexture() != NULL)
			this->HoverSprite.ChangeSize(D3DXVECTOR2(ButtonSpriteWidth * Scale.x,ButtonSpriteHeight * Scale.y));
		if (ClickedSprite.getTexture() != NULL)
			this->ClickedSprite.ChangeSize(D3DXVECTOR2(ButtonSpriteWidth * Scale.x,ButtonSpriteHeight * Scale.y));
		break;
	case 1:
		this->Scale = Scale;
		this->IdleFont.ChangeScale(Scale.x);
		this->HoverFont.ChangeScale(Scale.x);
		this->ClickedFont.ChangeScale(Scale.x);
		break;
	case 2:
		this->Scale = Scale;
		if (IdleSprite.getTexture() != NULL)
			this->IdleSprite.ChangeSize(D3DXVECTOR2(ButtonSpriteWidth * Scale.x,ButtonSpriteHeight * Scale.y));
		if (HoverSprite.getTexture() != NULL)
			this->HoverSprite.ChangeSize(D3DXVECTOR2(ButtonSpriteWidth * Scale.x,ButtonSpriteHeight * Scale.y));
		if (ClickedSprite.getTexture() != NULL)
			this->ClickedSprite.ChangeSize(D3DXVECTOR2(ButtonSpriteWidth * Scale.x,ButtonSpriteHeight * Scale.y));
		this->IdleFont.ChangeScale(Scale.x);
		this->IdleFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->IdleFont.GetRectDimensions(IdleFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->IdleFont.GetRectDimensions(IdleFontText).y/2));
		this->HoverFont.ChangeScale(Scale.x);
		this->HoverFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).y/2));
		this->ClickedFont.ChangeScale(Scale.x);
		this->ClickedFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).y/2));
		break;
	}
}

bool Button::Update(float MouseX, float MouseY, bool MouseClicked)
{
	D3DXVECTOR2 TempSize = D3DXVECTOR2(ButtonSpriteWidth,ButtonSpriteHeight);
	if (ButtonType != 1)
	{
		this->ButtonSpriteHeight = this->ButtonSpriteHeight * this->Scale.y;
		this->ButtonSpriteWidth = this->ButtonSpriteWidth * this->Scale.x;
	}

	switch(ButtonStage)
	{
	case 0:
		if((MouseX >= PositionX && MouseX <= (PositionX + ButtonSpriteWidth)) && (MouseY >= PositionY && MouseY <= (PositionY + ButtonSpriteHeight)))
		{
			if (!MouseClicked)
			{
				if (ButtonType == 1)
				{
					this->ButtonSpriteHeight = this->HoverFont.GetRectDimensions(this->HoverFontText).y;
					this->ButtonSpriteWidth = this->HoverFont.GetRectDimensions(this->HoverFontText).x;
				}
				ButtonStage = 1;
			}
		}
		break;
	case 1:
		if((MouseX >= PositionX && MouseX <= (PositionX + ButtonSpriteWidth)) && (MouseY >= PositionY && MouseY <= (PositionY + ButtonSpriteHeight)))
		{
			if(MouseClicked)
			{
				if (ButtonType == 1)
				{
					this->ButtonSpriteHeight = this->ClickedFont.GetRectDimensions(this->ClickedFontText).y;
					this->ButtonSpriteWidth = this->ClickedFont.GetRectDimensions(this->ClickedFontText).x;
				}
				ButtonStage = 2;
			}
		}
		else
		{
			if (ButtonType == 1)
			{
				this->ButtonSpriteHeight = this->IdleFont.GetRectDimensions(this->IdleFontText).y;
				this->ButtonSpriteWidth = this->IdleFont.GetRectDimensions(this->IdleFontText).x;
			}
			ButtonStage = 0;
		}
		break;
	case 2:
		if((MouseX >= PositionX && MouseX <= (PositionX + ButtonSpriteWidth)) && (MouseY >= PositionY && MouseY <= (PositionY + ButtonSpriteHeight)))
		{
			if(!MouseClicked)
			{
				ButtonStage = 3;
			}
		}
		else
		{
			if (ButtonType == 1)
			{
				this->ButtonSpriteHeight = this->IdleFont.GetRectDimensions(this->IdleFontText).y;
				this->ButtonSpriteWidth = this->IdleFont.GetRectDimensions(this->IdleFontText).x;
			}
			ButtonStage = 0;
		}
		break;
	}
	if(ButtonStage == 3)
	{
		if (ButtonType == 1)
		{
			this->ButtonSpriteHeight = this->IdleFont.GetRectDimensions(this->IdleFontText).y;
			this->ButtonSpriteWidth = this->IdleFont.GetRectDimensions(this->IdleFontText).x;
		}
		ButtonStage = 0;
		if (ButtonType != 1)
		{
			this->ButtonSpriteWidth = TempSize.x;
			this->ButtonSpriteHeight = TempSize.y;
		}
		return true;
	}
	else
	{
		if (ButtonType != 1)
		{
			this->ButtonSpriteWidth = TempSize.x;
			this->ButtonSpriteHeight = TempSize.y;
		}
		return false;
	}
}

void Button::Draw()
{
	
	switch(ButtonStage)
	{
		case 0:
			switch(ButtonType)
			{
			case 0:
				this->IdleSprite.Draw();
				break;
			case 1:
				this->IdleFont.Draw(IdleFontText);
				break;
			case 2:
				this->IdleSprite.Draw();
				this->IdleFont.Draw(IdleFontText);
				break;
			}
			break;
		case 1:
			switch(ButtonType)
			{
			case 0:
				this->HoverSprite.Draw();
				break;
			case 1:
				this->HoverFont.Draw(HoverFontText);
				break;
			case 2:
				this->HoverSprite.Draw();
				this->HoverFont.Draw(HoverFontText);
				break;
			}
			break;
		case 2:
			switch(ButtonType)
			{
			case 0:
				this->ClickedSprite.Draw();
				break;
			case 1:
				this->ClickedFont.Draw(ClickedFontText);
				break;
			case 2:
				this->ClickedSprite.Draw();
				this->ClickedFont.Draw(ClickedFontText);
				break;
			}
			break;
	}
}