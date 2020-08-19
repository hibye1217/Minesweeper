#pragma once
#include "Object.h"

class Font : public Object
{
private :
	LPD3DXFONT pFont;
	D3DXFONT_DESC desc;
	char str[STRING_LENGTH];

	D3DCOLOR color;

public :
	Font();
	Font(const char* text, int x, int y, int width, int height, D3DCOLOR c);
	~Font();

	void Render();
	void setText(const char* text);
	void setText(char* text);
	
	void setPos(int x, int y);
	void setHeight(int height);
	void setWidth(int width);

	void setColor(D3DCOLOR c);
};