#pragma once
#include "Object.h"
#include "Sprite.h"

class Ad : public Object {
private:
	int currentNum;

	float adTimer;

	bool renderAd, renderX;

	Sprite* ad[AD_COUNT];
	Sprite* X;
public:
	Ad();
	~Ad();

	void Update(float dTime);
	void Render();

	void setNum(int num);
	int getNum();
	bool getRenderX();
	Sprite* getX();

	void ShowAd();
	void ClickX();
};