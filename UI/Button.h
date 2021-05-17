// Used for clickedable buttons in a GUI
// By Michael Franklin

#ifndef Button_h_
#define Button_h_

#include "sprite.h"
#include "Font.h"

class Button
{
private:
	// idle
	sprite IdleSprite;
	Text IdleFont;
	char* IdleFontText;

	// hover
	sprite HoverSprite;
	Text HoverFont;
	char* HoverFontText;

	// clicked
	sprite ClickedSprite;
	Text ClickedFont;
	char* ClickedFontText;

	float PositionX;
	float PositionY;

	float ButtonSpriteHeight;
	float ButtonSpriteWidth;

	D3DXVECTOR2 Scale;

	int ButtonStage;
	int ButtonType;
	IDirect3DDevice9* Device;
public:
	~Button()
	{	
		this->IdleFontText = 0;
		delete this->IdleFontText;
		this->HoverFontText = 0;
		delete this->HoverFontText;
		this->ClickedFontText = 0;
		delete this->ClickedFontText;
		Device = 0;
		delete Device;
	}

	// call this only once in setup for each button you wish to make.
	// pass is the d3dxdevice for the window
	// all the sprite filepaths, all sprites used should be the same dimensions
	// pass it the width and height for the sprites, once again should be the same
	// for all three. also pass it the position you want the button to be and the
	// x and y scaling of the button. Scale should be 1 if no scale is to be
	// applied. finally pass it the color tint you want to put on the button
	// white is passed if no tint is to be applied.
	// Note: one sprite can be used all three times for each button state.
	void Create(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 IdleTexture, LPDIRECT3DTEXTURE9 HoverTexture, LPDIRECT3DTEXTURE9 ClickedTexture, int ButtonSpriteWidth, int ButtonSpriteHeight, float PositionX, float PositionY, D3DXVECTOR2 Scale = D3DXVECTOR2(1,1));
	void Create(LPDIRECT3DDEVICE9 Device, int IdleFontSize, int IdleFontWeight, bool IdleFontItalic,char IdleFontName[], DWORD IdleFontColor, char IdleFontText[], int HoverFontSize, int HoverFontWeight, bool HoverFontItalic,char HoverFontName[], DWORD HoverFontColor, char HoverFontText[], int ClickedFontSize, int ClickedFontWeight, bool ClickedFontItalic,char ClickedFontName[], DWORD ClickedFontColor, char ClickedFontText[], float PositionX, float PositionY, float Scale = 1);
	void Create(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 IdleTexture, LPDIRECT3DTEXTURE9 HoverTexture, LPDIRECT3DTEXTURE9 ClickedTexture, int IdleFontWeight, bool IdleFontItalic,char IdleFontName[], DWORD IdleFontColor, char IdleFontText[], int HoverFontWeight, bool HoverFontItalic,char HoverFontName[], DWORD HoverFontColor, char HoverFontText[], int ClickedFontWeight, bool ClickedFontItalic,char ClickedFontName[], DWORD ClickedFontColor, char ClickedFontText[], int ButtonSpriteWidth, int ButtonSpriteHeight, float PositionX, float PositionY, D3DXVECTOR2 Scale = D3DXVECTOR2(1,1));
	// run this every time you want to button to check for mouse position
	// pass it the mouse x and y position accourding to the client window and a bool
	// of if the mouse is clicked or not
	bool Update(float MouseX, float MouseY, bool MouseClicked);

	void ChangeSprite(LPDIRECT3DTEXTURE9 IdleTexture, LPDIRECT3DTEXTURE9 HoverTexture, LPDIRECT3DTEXTURE9 ClickedTexture);
	void ChangeScale(D3DXVECTOR2 Scale);

	inline void ChangeButtonText(char IdleText[], char HoverText[], char ClickedText[])
	{
		IdleFontText = IdleText;
		HoverFontText = HoverText;
		ClickedFontText = ClickedText;
		D3DXVECTOR2 TempVec;
		this->IdleFont.ChangeSize((40 * ButtonSpriteWidth)/200.0);
		TempVec = this->IdleFont.GetRectDimensions(this->IdleFontText);
		while(TempVec.x > ButtonSpriteWidth * Scale.x || TempVec.y > ButtonSpriteHeight * Scale.y)
		{
			this->IdleFont.ChangeSize(this->IdleFont.GetSize()-1);
			TempVec = this->IdleFont.GetRectDimensions(this->IdleFontText);
		};
		this->IdleFont.ChangePosition((PositionX+(ButtonSpriteWidth*Scale.x/2))-(this->IdleFont.GetRectDimensions(IdleFontText).x/2),(PositionY+(ButtonSpriteHeight*Scale.y/2))-(this->IdleFont.GetRectDimensions(IdleFontText).y/2));
		this->HoverFont.ChangeSize((40 * ButtonSpriteWidth)/200.0);
		TempVec = this->HoverFont.GetRectDimensions(this->HoverFontText);
		while(TempVec.x > ButtonSpriteWidth * Scale.x || TempVec.y > ButtonSpriteHeight * Scale.y)
		{
			this->HoverFont.ChangeSize(this->HoverFont.GetSize()-1);
			TempVec = this->HoverFont.GetRectDimensions(this->HoverFontText);
		};
		this->HoverFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).y/2));
		this->ClickedFont.ChangeSize((40 * ButtonSpriteWidth)/200.0);
		TempVec = this->ClickedFont.GetRectDimensions(this->ClickedFontText);
		while(TempVec.x > ButtonSpriteWidth * Scale.x || TempVec.y > ButtonSpriteHeight * Scale.y)
		{
			this->ClickedFont.ChangeSize(this->ClickedFont.GetSize()-1);
			TempVec = this->ClickedFont.GetRectDimensions(this->ClickedFontText);
		};
		this->ClickedFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).y/2));
	}
	inline void ChangePosition(D3DXVECTOR2 Position)
	{
		PositionX = Position.x;
		PositionY = Position.y;
		if (ButtonType == 0 || ButtonType == 2)
		{
			IdleSprite.Move(Position);
			if (ButtonType == 2)
				this->IdleFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->IdleFont.GetRectDimensions(IdleFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->IdleFont.GetRectDimensions(IdleFontText).y/2));
			HoverSprite.Move(Position);
			if (ButtonType == 2)
				this->HoverFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->HoverFont.GetRectDimensions(HoverFontText).y/2));
			ClickedSprite.Move(Position);
			if (ButtonType == 2)
				this->ClickedFont.ChangePosition((PositionX+((ButtonSpriteWidth * Scale.x)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).x/2),(PositionY+((ButtonSpriteHeight * Scale.y)/2))-(this->ClickedFont.GetRectDimensions(ClickedFontText).y/2));
		}
		else
		{
			IdleFont.ChangePosition(PositionX,PositionY);
			HoverFont.ChangePosition(PositionX,PositionY);
			ClickedFont.ChangePosition(PositionX,PositionY);
		}
	}
	inline Text* GetFont(int ButtonState)
	{
		switch(ButtonState)
		{
		case 0:
			return &IdleFont;
			break;
		case 1:
			return &HoverFont;
			break;
		case 2:
			return &ClickedFont;
			break;
		default:
			return NULL;
			break;
		}
	}
	inline D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(PositionX,PositionY);
	}
	inline D3DXVECTOR2 GetSize()
	{
		return D3DXVECTOR2(ButtonSpriteWidth*Scale.x,ButtonSpriteHeight*Scale.y);
	}
	inline void ChangeButtonStage(int ButtonStage)
	{
		this->ButtonStage = ButtonStage;
	}
	inline int GetButtonStage()
	{
		return ButtonStage;
	}
	// call this in between the start scene and end scene in display. draws the button
	void Draw();
};

#endif