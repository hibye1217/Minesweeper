#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Font.h"
#include "Animation.h"
#include "MainScene.h"
#include <vector>
//Object Headers
#include "Background.h"
#include "Block.h"
#include "Ad.h"

class GameScene : public Scene {
private :
	bool initBomb;
	int width, height, bombCount;
	int newWidth, newHeight, newBombCount;

	int flagCount, leftSpace;

	bool gameOver, gameClear;

	float timer;

	std::vector< std::vector<Block*> > blockMap;
	std::vector< std::vector<bool> > checkMap;

	Background* background;

	Font* timeText;
	Font* flagText;
	Font* spaceText;

	Font* widthText;
	Font* heightText;
	Font* bombText;

	Ad* ad;
public :
	GameScene();
	~GameScene();

	void Render();
	void Update(float dTime);

	void SetGame(int h, int w, int b);
	void SetBomb(int y, int x);

	int getAdjacentBombCount(int y, int x);
	int getAdjacentFlagCount(int y, int x);
	int getAdjacentCheckCount(int y, int x);

	void OpenSpace(int y, int x, bool check);
	void ExplodeBomb(int y, int x);
	bool RearrangeBomb(int y, int x, int Y, int X, int space, int bomb);
	bool ValidateBomb(int y, int x);

	void TimerUpdate(float dTime);
	void TextUpdate();
	void ClickCheck();
	void KeyCheck();
	void CheckValidSetting();
	void CheckEnd();

	void ShowAd();
};