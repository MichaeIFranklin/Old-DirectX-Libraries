// Basically for static or moving/animated fonts
// By Michael Franklin

#ifndef Font_h_
#define Font_h_

#include <d3dx9.h>
#include <string>

class Text
{
private:
	LPDIRECT3DDEVICE9 Device;
	ID3DXSprite* TextSprite;
	RECT TextRect;
	ID3DXFont* TextFont;
	bool Italic;
	char* Font;
	UINT Weight;
	float Size;
	float Scale;
	DWORD Color;
	float PositionX;
	float PositionY;

	void Update();
public:
	// these inline funtions should be self explainitory, thier names say what they do
	// "GetPosition" gets the position of the text... Simple right?
	// few small notes: ChangeItalic, pass this true if you want the font to be Italic
	// also with ChangeWeight I have found that 20 is the smallest number you can pass it
	// and still have clear text.
	Text()
	{
		TextSprite = NULL;
		TextFont = NULL;
	}
	~Text()
	{
		if(TextSprite != NULL)
		{
			TextFont->Release();
			TextFont = 0;
		}
		if (TextFont != NULL)
		{
			TextFont->Release();
			TextFont = 0;
		}
		Font = 0;
		delete Font;
	}

	inline D3DXVECTOR2 GetPosition()
	{
		return D3DXVECTOR2(PositionX,PositionY);
	}
	inline D3DXVECTOR2 GetRectDimensions(char Buffer[])
	{
		RECT TempRect;
		TempRect.left = 0;
		TempRect.top = 0;

		this->TextFont->DrawTextA(this->TextSprite, Buffer, -1, &TempRect, DT_CALCRECT | DT_TOP| DT_LEFT, Color);

		return D3DXVECTOR2(TempRect.right,TempRect.bottom);
	}
	inline float GetSize()
	{
		return Size;
	}
	inline UINT GetWeight()
	{
		return Weight;
	}
	inline bool GetItalic()
	{
		return Italic;
	}
	inline char* GetFont()
	{
		return Font;
	}
	inline void ChangePosition(float PositionX, float PositionY)
	{
		this->PositionX = PositionX;
		this->PositionY = PositionY;
	}
	inline void ChangeFont(char Font[])
	{
		this->Font = Font;
		Update();
	}
	inline void ChangeSize(float Size)
	{
		this->Size = Size;
		Update();
	}
	inline void ChangeScale(float Scale)
	{
		this->Scale= Scale;
		Update();
	}
	inline void ChangeWeight(UINT Weight)
	{
		this->Weight = Weight;
		Update();
	}
	inline void ChangeColor(DWORD Color)
	{
		this->Color = Color;
		Update();
	}
	inline void ChangeItalic(bool Italic)
	{
		this->Italic = Italic;
		Update();
	}

	// call this in setup for every different font you want. Remeber you can use the functions
	// above to change to font around during run time.
	// pass it the d3dx device for the client window. also pass it the size that you want the font to be
	// as well as the weight of the font. Along with that a bool that should be set to true if you wwant
	// the font to be Italic or false if not. Pass it a string of the name of the font you want to use
	// make sure you get it the same spelling and case as it shows in MS Word. Example: "Times New Roman"
	// finally pass it the color you want to be and the postion of the top left corner of the textbox
	// you want the text to show up in when draw. Note: the "textbox" is invisible.
	void SetupFont(IDirect3DDevice9* Device, int Size,UINT Weight, bool Italic,char Font[], DWORD Color, float PositionX, float PositionY, float Scale = 1);

	// call this in between the start scene and end scene in display. Pass it the string or char array
	// you want to be printed to the screen
	void Draw(char Buffer[], int Alignment = 0);
};

#endif