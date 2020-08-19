#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "GameScene.h"

class MainScene : public Scene
{
public :
	MainScene();
	~MainScene();

	Sprite* background;

	Sprite* startButtonEnglish;
	Sprite* startButtonKorean;

	Sprite* exitButtonEnglish;
	Sprite* exitButtonKorean;

	Sprite* titleEnglish;
	Sprite* titleKorean;

	Sprite* hackFace;
	Sprite* adFace;
	Sprite* luckFace;
	Sprite* koreanFace;

	void Render();
	void Update(float dTime);
};