#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene() {
	background = new Sprite("Resources/Image/Background_Main.png");
	AddObject(background);
	background->setPos(0, 0);

	titleEnglish = new Sprite("Resources/Image/Title_English.png");
	AddObject(titleEnglish);
	titleEnglish->setPos(MiddleX(titleEnglish->getWidth(), 0, SCREEN_WIDTH), 0);

	titleKorean = new Sprite("Resources/Image/Title_Korean.png");
	AddObject(titleKorean);
	titleKorean->setPos(MiddleX(titleKorean->getWidth(), 0, SCREEN_WIDTH), 0);

	startButtonEnglish = new Sprite("Resources/Image/Main_Start_English.png");
	AddObject(startButtonEnglish);
	startButtonEnglish->setPos(MiddleX(startButtonEnglish->getWidth(), 0, SCREEN_WIDTH / 2), 350);

	startButtonKorean = new Sprite("Resources/Image/Main_Start_Korean.png");
	AddObject(startButtonKorean);
	startButtonKorean->setPos(MiddleX(startButtonKorean->getWidth(), 0, SCREEN_WIDTH / 2), 350);

	exitButtonEnglish = new Sprite("Resources/Image/Main_Exit_English.png");
	AddObject(exitButtonEnglish);
	exitButtonEnglish->setPos(MiddleX(exitButtonEnglish->getWidth(), SCREEN_WIDTH / 2, SCREEN_WIDTH), 350);

	exitButtonKorean = new Sprite("Resources/Image/Main_Exit_Korean.png");
	AddObject(exitButtonKorean);
	exitButtonKorean->setPos(MiddleX(exitButtonKorean->getWidth(), SCREEN_WIDTH / 2, SCREEN_WIDTH), 350);

	hackFace = new Sprite("Resources/Image/Face_Smile_Sunglass.png");
	AddObject(hackFace);
	hackFace->setPos(FACE_MARGIN_X, HEIGHT - FACE_MARGIN_Y - FACE_SIZE);

	adFace = new Sprite("Resources/Image/Face_Died.png");
	AddObject(adFace);
	adFace->setPos(FACE_MARGIN_X * 2 + FACE_SIZE, HEIGHT - FACE_MARGIN_Y - FACE_SIZE);

	luckFace = new Sprite("Resources/Image/Face_Smile.png");
	AddObject(luckFace);
	luckFace->setPos(FACE_MARGIN_X * 3 + FACE_SIZE * 2, HEIGHT - FACE_MARGIN_Y - FACE_SIZE);

	koreanFace = new Sprite("Resources/Image/Face_Surprised.png");
	AddObject(koreanFace);
	koreanFace->setPos(FACE_MARGIN_X * 4 + FACE_SIZE * 3, HEIGHT - FACE_MARGIN_Y - FACE_SIZE);

	//soundManager->LoadWave("Resources/Sound/MusicSurround.wav", "Background");
	//soundManager->Play("Background", true);
}

MainScene::~MainScene() {

}

void MainScene::Render() {
	background->Render();
	if (koreanMode) titleKorean->Render();
	else titleEnglish->Render();
	if (koreanMode) startButtonKorean->Render();
	else startButtonEnglish->Render();
	if (koreanMode) exitButtonKorean->Render();
	else exitButtonEnglish->Render();

	if (hackMode) hackFace->Render();
	if (adMode) adFace->Render();
	if (luckMode) luckFace->Render();
	if (koreanMode) koreanFace->Render();
}

void MainScene::Update(float dTime) {
	Scene::Update(dTime);

	if (inputManager->GetKeyState('H') == KEY_ON) hackMode = true;
	else hackMode = false;
	if (inputManager->GetKeyState('A') == KEY_ON) adMode = true;
	else adMode = false;
	if (inputManager->GetKeyState('C') == KEY_ON) luckMode = true;
	else luckMode = false;
	if (inputManager->GetKeyState('K') == KEY_ON) koreanMode = true;
	else koreanMode = false;

	if (inputManager->GetKeyState(VK_LBUTTON) == KEY_DOWN) {

		if (startButtonEnglish->IsPointInRect(inputManager->GetMousePos()) || 
			startButtonKorean->IsPointInRect(inputManager->GetMousePos())) {
			std::cout << " Hack  Mode: " << (hackMode ? "ON " : "OFF") << std::endl;
			std::cout << "  Ad   Mode: " << (adMode ? "ON " : "OFF") << std::endl;
			std::cout << " Luck  Mode: " << (luckMode ? "ON " : "OFF") << std::endl;
			std::cout << "Korean Mode: " << (koreanMode ? "ON " : "OFF") << std::endl;

			sceneManager->ChangeScene(new GameScene());
			return;
		}

		if (exitButtonEnglish->IsPointInRect(inputManager->GetMousePos()) ||
			exitButtonKorean->IsPointInRect(inputManager->GetMousePos())) {
			PostQuitMessage(0);
			return;
		}

	}
}