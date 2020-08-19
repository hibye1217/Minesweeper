#pragma once
#include "Object.h"
#include "Sprite.h"

class Block : public Object {
private:
	Sprite* block[BLOCK_COUNT];

	int number;
public:
	Block();
	~Block();

	void Update(float dTime);
	void Render();

	void setNumber(int num);
	int getNumber();
};