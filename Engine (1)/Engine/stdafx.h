#pragma once

//프로그램 세팅
#define CONSOLE_ON true
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BG_COLOR D3DCOLOR_ARGB(255,255,255,255)
#define PROGRAM_NAME TEXT("IQ 200 이상만 이 게임을 깰 수 있습니다.")
#define CONSOLE_NAME TEXT("상위 0.1%만 이 게임을 깰 수 있습니다.")

//상수
#define WIDTH 1008
#define HEIGHT 729
#define MAP_WIDTH 729
#define MAP_HEIGHT 729
#define OPTION_WIDTH 279
#define OPTION_HEIGHT 729
#define BLOCK_SIZE 2952
#define FACE_SIZE 60
#define FACE_MARGIN_X 20
#define FACE_MARGIN_Y 20
#define AD_MARGIN_X 938
#define AD_MARGIN_Y 20

#define OPEN 0
#define NORMAL_CLOSED 9
#define BOMB_CLOSED 10
#define NORMAL_FLAG 11
#define BOMB_FLAG 12
#define BOMB_SHOW 13
#define BOMB_EXPLODE 14
#define NORMAL_SHOW 15

#define KEY_NONE 0
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_ON 3

#define FONT_WIDTH 14
#define FONT_HEIGHT 40
#define FONT_MARGIN 10

#define BLOCK_COUNT 16
#define DIRECTION_COUNT 8
#define AD_COUNT 5
#define STRING_LENGTH 50

#define SAFE_RELEASE(p) {if(p) {p->Release(); (p) = NULL;}}
#define SAFE_DELETE(p) {if(p) {delete (p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p){delete [](p); (p) = nullptr;}}

#define MiddleX(w,l,r) ((r-l) - w) / 2 + l
#define MiddleY(h,l,r) ((r-l) - h) / 2 + l

#define outOfRange(y,x) (0 > x || x >= width || 0 > y || y >= height)

#define isOpened(y,x) (0 <= blockMap[y][x]->getNumber() && blockMap[y][x]->getNumber() <= 8)
#define isClosed(y,x) (9 <= blockMap[y][x]->getNumber() && blockMap[y][x]->getNumber() <= 10)
#define isFlaged(y,x) (11 <= blockMap[y][x]->getNumber() && blockMap[y][x]->getNumber() <= 12)
#define isBomb(y,x) (blockMap[y][x]->getNumber() == 10 || blockMap[y][x]->getNumber() == 12)

//윈도우 헤더
#include <Windows.h>
#include <mmsystem.h>

//다이렉트X 헤더
#include <d3d9.h>
#include <d3dx9.h>
#include <dsound.h>

//디버그 헤더
#include <cstdio>
#include <cstring>
#include <iostream>

//게임 매니저 헤더
#include "TextureManager.h"
#include "SceneManager.h"
#include "InputManager.h"
//#include "SoundManager.h"

//라이브러리
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9d.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")

//전역 변수
extern LPDIRECT3D9 pd3d;
extern D3DPRESENT_PARAMETERS d3dpp;
extern LPDIRECT3DDEVICE9 pd3dDevice;
extern LPD3DXSPRITE pd3dSprite;
extern HWND hWnd;
extern SceneManager* sceneManager;
extern TextureManager* textureManager;
extern InputManager* inputManager;
//extern SoundManager* soundManager;

extern const int dx[DIRECTION_COUNT];
extern const int dy[DIRECTION_COUNT];

extern bool hackMode;
extern bool adMode;
extern bool luckMode;
extern bool koreanMode;