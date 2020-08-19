#include "stdafx.h"
#include "Block.h"

Block::Block() {
	number = NORMAL_CLOSED;

	block[0] = new Sprite("Resources/Image/Block_Opened.png");
	block[1] = new Sprite("Resources/Image/Block_1.png");
	block[2] = new Sprite("Resources/Image/Block_2.png");
	block[3] = new Sprite("Resources/Image/Block_3.png");
	block[4] = new Sprite("Resources/Image/Block_4.png");
	block[5] = new Sprite("Resources/Image/Block_5.png");
	block[6] = new Sprite("Resources/Image/Block_6.png");
	block[7] = new Sprite("Resources/Image/Block_7.png");
	block[8] = new Sprite("Resources/Image/Block_8.png");
	block[9] = new Sprite("Resources/Image/Block_Closed_Normal.png");
	block[10] = new Sprite(hackMode ? "Resources/Image/Block_Closed_Bomb.png" : "Resources/Image/Block_Closed_Normal.png");
	block[11] = new Sprite("Resources/Image/Block_Flag_Normal.png");
	block[12] = new Sprite(hackMode ? "Resources/Image/Block_Flag_Bomb.png" : "Resources/Image/Block_Flag_Normal.png");
	block[13] = new Sprite("Resources/Image/Block_Open_Bomb.png");
	block[14] = new Sprite("Resources/Image/Block_Open_Bomb_Exploded.png");
	block[15] = new Sprite("Resources/Image/Block_Open_Normal_Flaged.png");

	for (int i = 0; i < BLOCK_COUNT; i++) {
		AddChild(block[i]);
	}
}

Block::~Block() {
	for (int i = 0; i < BLOCK_COUNT; i++) {
		SAFE_DELETE(block[i]);
	}
}

void Block::Update(float dTime) {
	Object::Update(dTime);
	rect = block[number]->getRect();
}

void Block::Render() {
	Object::Render();
	block[number]->Render();
}

void Block::setNumber(int num) {
	number = num;
}

int Block::getNumber() {
	return number;
}