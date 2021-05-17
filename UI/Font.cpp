// The cpp for my Font Class
// By Michael Franklin

#include "Font.h"

void Text::SetupFont(IDirect3DDevice9* Device, int Size,UINT Weight, bool Italic,char Font[], DWORD Color, float PositionX, float PositionY, float Scale)
{
	this->Device = Device;
	TextFont = NULL;
	TextSprite = NULL;
	this->Size = Size;
	this->Scale = Scale;
	this->Weight = Weight;
	this->Italic = Italic;
	this->Color = Color;
	this->PositionX = PositionX;
	this->PositionY = PositionY;
	this->Font = Font;
	Update();
}

void Text::Update()
{
	char FontBuffer[500];
	for (int iIterator = 0;iIterator < sizeof(FontBuffer);iIterator++)
	{
		FontBuffer[iIterator] = 0;
	}
	for (int iIterator = 0;iIterator < sizeof(Font);iIterator++)
	{
		FontBuffer[iIterator] = Font[iIterator];
	}
	if (this->TextFont != NULL)
	{
		this->TextFont->Release();
		TextFont = NULL;
	}
	D3DXCreateFont( this->Device, Size*Scale, 0, Weight, 0, Italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT(FontBuffer), &this->TextFont );
}

void Text::Draw(char Buffer[], int Alignment)
{
	int TempSize =GetSize();

	TextRect.left = this->PositionX;
	TextRect.top = this->PositionY;

	switch(Alignment)
	{
	case 0:
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_CALCRECT | DT_TOP, Color);	
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_TOP | DT_LEFT, Color);
		break;
	case 1:
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_CALCRECT | DT_TOP, Color);	
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_TOP | DT_CENTER, Color);
		break;
	case 2:
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_CALCRECT | DT_TOP, Color);	
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_TOP | DT_RIGHT, Color);
		break;
	case 3:
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_CALCRECT | DT_TOP | DT_CENTER, Color);	
		TextFont->DrawTextA(TextSprite, Buffer, -1, &TextRect, DT_TOP | DT_LEFT, Color);
		break;
	}
}