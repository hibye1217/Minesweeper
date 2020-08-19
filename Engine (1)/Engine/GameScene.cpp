#include "stdafx.h"
#include "GameScene.h"

GameScene::GameScene() {
	newWidth = 9;
	newHeight = 9;
	newBombCount = 10;

	background = new Background();
	background->setPos(0, 0);
	AddObject(background);

	timeText = new Font(!koreanMode ? "Time: " : "½Ã°¢: ", MAP_WIDTH + FONT_MARGIN, 0,
		FONT_WIDTH, FONT_HEIGHT, D3DCOLOR_ARGB(255, 0, 0, 0));
	flagText = new Font(!koreanMode ? "Flag: " : "±ê¹ß: ", MAP_WIDTH + FONT_MARGIN, FONT_HEIGHT,
		FONT_WIDTH, FONT_HEIGHT, D3DCOLOR_ARGB(255, 0, 0, 0));
	spaceText = new Font(!koreanMode ? "Left: " : "¿ÞÂÊ: ", MAP_WIDTH + FONT_MARGIN, FONT_HEIGHT * 2,
		FONT_WIDTH, FONT_HEIGHT, D3DCOLOR_ARGB(255, 0, 0, 0));
	widthText = new Font(!koreanMode ? "Width: " : "Æø: ", MAP_WIDTH + FONT_MARGIN, OPTION_HEIGHT / 2,
		FONT_WIDTH, FONT_HEIGHT, D3DCOLOR_ARGB(255, 0, 0, 0));
	heightText = new Font(!koreanMode ? "Height: " : "½ÅÀå: ", MAP_WIDTH + FONT_MARGIN, OPTION_HEIGHT / 2 + FONT_HEIGHT,
		FONT_WIDTH, FONT_HEIGHT, D3DCOLOR_ARGB(255, 0, 0, 0));
	bombText = new Font(!koreanMode ? "Bomb: " : "ÆøÅº", MAP_WIDTH + FONT_MARGIN, OPTION_HEIGHT / 2 + FONT_HEIGHT * 2,
		FONT_WIDTH, FONT_HEIGHT, D3DCOLOR_ARGB(255, 0, 0, 0));

	ad = new Ad();
	ad->setPos(0, 0);
	AddObject(ad);

	SetGame(newWidth, newHeight, newBombCount);

	if (adMode) ad->ShowAd();
}

GameScene::~GameScene() {

}

void GameScene::SetGame(int h, int w, int b) {
	width = w;
	height = h;
	bombCount = b;
	initBomb = false;

	timer = 0.0;
	flagCount = 0;
	leftSpace = width * height - bombCount;

	gameOver = false;
	gameClear = false;

	blockMap.resize(height);
	for (int i = 0; i < height; i++) {
		blockMap[i].resize(width);
		for (int j = 0; j < width; j++) {
			blockMap[i][j] = new Block();
			AddObject(blockMap[i][j]);
			//std::cout << i << ", " << j << " : " << (float)j * MAP_WIDTH / width << ", " << (float)i * MAP_HEIGHT / height << std::endl;
			blockMap[i][j]->setScale((float)MAP_WIDTH / BLOCK_SIZE / width, (float)MAP_HEIGHT / BLOCK_SIZE / height);
			blockMap[i][j]->setPos((float)j * MAP_WIDTH / width, (float)i * MAP_HEIGHT / height);
		}
	}

	checkMap.resize(height);
	for (int i = 0; i < height; i++) {
		checkMap[i].resize(width);
		for (int j = 0; j < width; j++) {
			checkMap[i][j] = false;
		}
	}

	if (adMode) ad->ShowAd();
}

void GameScene::SetBomb(int y, int x) {
	blockMap[y][x]->setNumber(BOMB_CLOSED);
	for (int k = 0; k < DIRECTION_COUNT; k++) {
		int X = x + dx[k], Y = y + dy[k];
		if (outOfRange(Y, X)) continue;
		blockMap[Y][X]->setNumber(BOMB_CLOSED);
	}

	for (int i = 0; i < bombCount; i++) {
		int x, y;
		y = rand() % height;
		x = rand() % width;

		while (blockMap[y][x]->getNumber() == BOMB_CLOSED) {
			y = rand() % height;
			x = rand() % width;
		}

		blockMap[y][x]->setNumber(BOMB_CLOSED);
	}

	blockMap[y][x]->setNumber(NORMAL_CLOSED);
	for (int k = 0; k < DIRECTION_COUNT; k++) {
		int X = x + dx[k], Y = y + dy[k];
		if (outOfRange(Y, X)) continue;
		blockMap[Y][X]->setNumber(NORMAL_CLOSED);
	}
	initBomb = true;
}

int GameScene::getAdjacentBombCount(int y, int x) {
	int count = 0;
	for (int k = 0; k < DIRECTION_COUNT; k++) {
		int X = x + dx[k], Y = y + dy[k];
		if (outOfRange(Y, X)) continue;

		if (isBomb(Y, X)) count += 1;
	}

	return count;
}

int GameScene::getAdjacentFlagCount(int y, int x) {
	int count = 0;
	for (int k = 0; k < DIRECTION_COUNT; k++) {
		int X = x + dx[k], Y = y + dy[k];
		if (outOfRange(Y, X)) continue;

		if (isFlaged(Y, X)) count += 1;
	}

	return count;
}

int GameScene::getAdjacentCheckCount(int y, int x) {
	int count = 0;
	for (int k = 0; k < DIRECTION_COUNT; k++) {
		int X = x + dx[k], Y = y + dy[k];
		if (outOfRange(Y, X)) continue;

		if (checkMap[Y][X]) count += 1;
	}

	return count;
}

void GameScene::OpenSpace(int y, int x, bool check) {	
	int bomb = getAdjacentBombCount(y, x);
	int flag = getAdjacentFlagCount(y, x);

	//std::cout << y << ' ' << x << " : " << bomb << ' ' << flag << std::endl;

	if (blockMap[y][x]->getNumber() == BOMB_CLOSED) {
		if (luckMode) {
			int leftSpace = 0, bombCount = 0;
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					if (isClosed(i, j) || isFlaged(i, j)) leftSpace += 1;
					if (isBomb(i, j)) bombCount += 1;
					checkMap[i][j] = false;
				}
			}
			bool _ = RearrangeBomb(0, 0, y, x, leftSpace, bombCount);
			if (checkMap[y][x]) {
				ExplodeBomb(y, x);
				return;
			}
			else {
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						switch (blockMap[i][j]->getNumber()) {
						case NORMAL_CLOSED:
						case BOMB_CLOSED:
							if (checkMap[i][j]) blockMap[i][j]->setNumber(BOMB_CLOSED);
							else blockMap[i][j]->setNumber(NORMAL_CLOSED);
							break;
						case NORMAL_FLAG:
						case BOMB_FLAG:
							if (checkMap[i][j]) blockMap[i][j]->setNumber(BOMB_FLAG);
							else blockMap[i][j]->setNumber(NORMAL_FLAG);
							break;
						}
					}
				}
			}
		}
		else {
			ExplodeBomb(y, x);
			return;
		}
	}

	if (isClosed(y, x)) leftSpace -= 1;

	bomb = getAdjacentBombCount(y, x);
	flag = getAdjacentFlagCount(y, x);

	blockMap[y][x]->setNumber(bomb);

	if (bomb == 0 ||
		(check && bomb == flag)) {
		for (int k = 0; k < DIRECTION_COUNT; k++) {
			int X = x + dx[k], Y = y + dy[k];
			if (outOfRange(Y, X)) continue;
			if (isOpened(Y, X)) continue;
			if (isFlaged(Y, X)) continue;
			if (gameOver || gameClear) return;

			OpenSpace(Y, X, false);
		}
	}
}

void GameScene::ExplodeBomb(int y, int x) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			switch (blockMap[i][j]->getNumber()) {
			case BOMB_CLOSED:
				blockMap[i][j]->setNumber(BOMB_SHOW);
				break;
			case NORMAL_FLAG:
				blockMap[i][j]->setNumber(NORMAL_SHOW);
				break;
			}
		}
	}

	blockMap[y][x]->setNumber(BOMB_EXPLODE);

	gameOver = true;
}

bool GameScene::RearrangeBomb(int y, int x, int Y, int X, int space, int bomb) {
	//std::cout << 'S' << ' ' << y << ' ' << x << ' ' << Y << ' ' << X << ' ' << space << ' ' << bomb << std::endl;
	if (x == width) {
		x = 0;
		y += 1;
		if (y == height) {
			//for (int i = 0; i < height; i++) {
				//for (int j = 0; j < width; j++) {
					//std::cout << checkMap[i][j] << ' ';
				//}
				//std::cout << std::endl;
			//}
			//std::cout << std::endl;
			if (bomb == 0 && checkMap[Y][X] == false) return true;
			else return false;
		}
	}

	if (isOpened(y, x)) {
		checkMap[y][x] = false;
		if (ValidateBomb(y, x)) return RearrangeBomb(y, x + 1, Y, X, space, bomb);
		return false;
	}

	bool res = false;
	if ((y == Y && x == X) || 
		(!isFlaged(y, x) && rand() % space >= bomb)) {
		if (!res && bomb != space) {
			checkMap[y][x] = false;
			if (ValidateBomb(y, x)) res = (res || RearrangeBomb(y, x + 1, Y, X, space - 1, bomb));
		}
		if (!res && bomb != 0) {
			checkMap[y][x] = true;
			if (ValidateBomb(y, x)) res = (res || RearrangeBomb(y, x + 1, Y, X, space - 1, bomb - 1));
		}
	}
	else {
		if (!res && bomb != 0) {
			checkMap[y][x] = true;
			if (ValidateBomb(y, x)) res = (res || RearrangeBomb(y, x + 1, Y, X, space - 1, bomb - 1));
		}
		if (!res && bomb != space) {
			checkMap[y][x] = false;
			if (ValidateBomb(y, x)) res = (res || RearrangeBomb(y, x + 1, Y, X, space - 1, bomb));
		}
	}

	//std::cout << 'E' << ' ' << y << ' ' << x << ' ' << Y << ' ' << X << ' ' << space << ' ' << bomb << std::endl;
	return res;
}

bool GameScene::ValidateBomb(int y, int x) {
	if (y < 1 || x < 1) return true;

	if (isOpened(y - 1, x - 1)) {
		int bomb = getAdjacentBombCount(y - 1, x - 1);
		int flag = getAdjacentCheckCount(y - 1, x - 1);
		if (bomb != flag) return false;
	}

	if (y == height - 1 && isOpened(y, x - 1)) {
		int bomb = getAdjacentBombCount(y, x - 1);
		int flag = getAdjacentCheckCount(y, x - 1);
		if (bomb != flag) return false;
	}

	if (x == width - 1 && isOpened(y - 1, x)) {
		int bomb = getAdjacentBombCount(y - 1, x);
		int flag = getAdjacentCheckCount(y - 1, x);
		if (bomb != flag) return false;
	}

	if ((y == height - 1 && x == width - 1) && isOpened(y, x)) {
		int bomb = getAdjacentBombCount(y, x);
		int flag = getAdjacentCheckCount(y, x);
		if (bomb != flag) return false;
	}

	return true;
}

void GameScene::Render() {
	background->Render();

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			blockMap[i][j]->Render();
		}
	}

	timeText->Render();
	flagText->Render();
	spaceText->Render();
	widthText->Render();
	heightText->Render();
	bombText->Render();

	if (adMode) ad->Render();
}

void GameScene::Update(float dTime) {
	Scene::Update(dTime);

	TimerUpdate(dTime);
	TextUpdate();
	ClickCheck();
	KeyCheck();
	CheckValidSetting();
	CheckEnd();
}

void GameScene::TimerUpdate(float dTime) {
	if (initBomb) {
		if (!(gameOver || gameClear)) {
			timer += dTime;
		}
	}
}

void GameScene::TextUpdate() {
	char timeStr[STRING_LENGTH] = "";
	char flagStr[STRING_LENGTH] = "";
	char spaceStr[STRING_LENGTH] = "";
	char widthStr[STRING_LENGTH] = "";
	char heightStr[STRING_LENGTH] = "";
	char bombStr[STRING_LENGTH] = "";

	sprintf(timeStr, "%s: %.3f %s.", !koreanMode ? "Time" : "½Ã°¢", timer, !koreanMode ? "sec" : "ºñ¼­");
	sprintf(flagStr, "%s: %d / %d", !koreanMode ? "Flag" : "±ê¹ß", flagCount, bombCount);
	sprintf(spaceStr, "%s: %d / %d", !koreanMode ? "Left" : "¿ÞÂÊ", leftSpace, width * height - bombCount);
	sprintf(widthStr, "%s: %d", !koreanMode ? "Width" : "Æø", newWidth);
	sprintf(heightStr, "%s: %d", !koreanMode ? "Height" : "½ÅÀå", newHeight);
	sprintf(bombStr, "%s: %d", !koreanMode ? "Bomb" : "ÆøÅº", newBombCount);

	//printf("%s %s %s\n%s %s %s\n", timeStr, flagStr, spaceStr, widthStr, heightStr, bombStr);

	timeText->setText(timeStr);
	flagText->setText(flagStr);
	spaceText->setText(spaceStr);
	widthText->setText(widthStr);
	heightText->setText(heightStr);
	bombText->setText(bombStr);
}

void GameScene::ClickCheck() {
	D3DXVECTOR2 mousePosition = inputManager->GetMousePos();
	if (inputManager->GetKeyState(VK_LBUTTON) == KEY_DOWN) {

		//std::cout << "Click" << std::endl;
		if (ad->getRenderX()) {
			if (ad->getX()->IsPointInRect(mousePosition)) {
				ad->ClickX();
			}
		}

		if (mousePosition.x > MAP_WIDTH) return;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (blockMap[i][j]->IsPointInRect(mousePosition)) {
					if (i + 1 != height && blockMap[i + 1][j]->IsPointInRect(mousePosition)) continue;
					if (j + 1 != width && blockMap[i][j + 1]->IsPointInRect(mousePosition)) continue;
					if (gameOver || gameClear) continue;

					if (!isFlaged(i, j)){
						if (!initBomb) SetBomb(i, j);
						if (isOpened(i, j)) OpenSpace(i, j, true);
						else OpenSpace(i, j, false);
						if (adMode) ad->ShowAd();
					}
				}
			}
		}

		return;
	}

	if (inputManager->GetKeyState(VK_RBUTTON) == KEY_DOWN) {
		//std::cout << "Crick" << std::endl;

		if (mousePosition.x > MAP_WIDTH) return;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (blockMap[i][j]->IsPointInRect(mousePosition)) {
					if (i + 1 != height && blockMap[i + 1][j]->IsPointInRect(mousePosition)) continue;
					if (j + 1 != width && blockMap[i][j + 1]->IsPointInRect(mousePosition)) continue;
					if (gameOver || gameClear) continue;

					switch (blockMap[i][j]->getNumber()) {
					case NORMAL_CLOSED:
						blockMap[i][j]->setNumber(NORMAL_FLAG);
						flagCount += 1;
						break;
					case BOMB_CLOSED:
						blockMap[i][j]->setNumber(BOMB_FLAG);
						flagCount += 1;
						break;
					case NORMAL_FLAG:
						blockMap[i][j]->setNumber(NORMAL_CLOSED);
						flagCount -= 1;
						break;
					case BOMB_FLAG:
						blockMap[i][j]->setNumber(BOMB_CLOSED);
						flagCount -= 1;
						break;
					}
					if (adMode) ad->ShowAd();
				}
			}
		}

		return;
	}
}

void GameScene::KeyCheck() {
	if (inputManager->GetKeyState('R') == KEY_DOWN) {
		SetGame(newWidth, newHeight, newBombCount);
		if (adMode) ad->ShowAd();
		return;
	}

	if (inputManager->GetKeyState(VK_UP) == KEY_DOWN) {
		if (newHeight > 5) newHeight -= 1;
		if (adMode) ad->ShowAd();
		return;
	}

	if (inputManager->GetKeyState(VK_DOWN) == KEY_DOWN) {
		newHeight += 1;
		if (adMode) ad->ShowAd();
		return;
	}

	if (inputManager->GetKeyState(VK_LEFT) == KEY_DOWN) {
		if (newWidth > 5) newWidth -= 1;
		if (adMode) ad->ShowAd();
		return;
	}

	if (inputManager->GetKeyState(VK_RIGHT) == KEY_DOWN) {
		newWidth += 1;
		if (adMode) ad->ShowAd();
		return;
	}

	if (inputManager->GetKeyState('N') == KEY_DOWN) {
		newBombCount -= 1;
		if (adMode) ad->ShowAd();
		return;
	}

	if (inputManager->GetKeyState('M') == KEY_DOWN) {
		newBombCount += 1;
		if (adMode) ad->ShowAd();
		return;
	}

	if (inputManager->GetKeyState(VK_ESCAPE) == KEY_DOWN) {
		sceneManager->ChangeScene(new MainScene());
		return;
	}
}

void GameScene::CheckValidSetting() {
	if (newWidth < 5) newWidth = 5;

	if (newHeight < 5) newHeight = 5;

	if (newBombCount > newWidth * newHeight - 9) newBombCount = newWidth * newHeight - 9;
	if (newBombCount < 1) newBombCount = 1;
}

void GameScene::CheckEnd() {
	if (leftSpace == 0) {
		gameClear = true;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				switch (blockMap[i][j]->getNumber()) {
				case BOMB_CLOSED:
					blockMap[i][j]->setNumber(BOMB_FLAG);
					break;
				}
			}
		}
	}
}