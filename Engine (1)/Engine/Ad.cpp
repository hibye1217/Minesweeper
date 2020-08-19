#include "stdafx.h"
#include "Ad.h"

Ad::Ad() {
	renderAd = false;
	renderX = false;
	adTimer = 0;
	currentNum = 0;

	ad[0] = new Sprite("Resources/Image/Ad_1.png");
	ad[1] = new Sprite("Resources/Image/Ad_2.png");
	ad[2] = new Sprite("Resources/Image/Ad_3.png");
	ad[3] = new Sprite("Resources/Image/Ad_4.png");
	ad[4] = new Sprite("Resources/Image/Ad_5.png");

	X = new Sprite("Resources/Image/Ad_X.png");
	X->setPos(AD_MARGIN_X, AD_MARGIN_Y);

	for (int i = 0; i < AD_COUNT; i++) {
		AddChild(ad[i]);
	}
	AddChild(X);
}

Ad::~Ad() {

}

void Ad::Render() {
	Object::Render();
	if (renderAd) ad[currentNum]->Render();
	if (renderX) X->Render();
}

void Ad::Update(float dTime) {
	Object::Update(dTime);
	if (renderAd) {
		adTimer += dTime;
		if (adTimer >= 5) renderX = true;
		//std::cout << adTimer << std::endl;
	}
}

void Ad::setNum(int num) {
	currentNum = num;
}

int Ad::getNum() {
	return currentNum;
}

bool Ad::getRenderX() {
	return renderX;
}

Sprite* Ad::getX() {
	return X;
}

void Ad::ShowAd() {
	currentNum = rand() % AD_COUNT;
	renderAd = true;
}

void Ad::ClickX() {
	renderAd = false;
	renderX = false;
	adTimer = 0;
}