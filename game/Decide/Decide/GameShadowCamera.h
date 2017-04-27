#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
//ゲームで使う影を作るためのカメラ
class GameShadowCamera :public GameObject
{
public:
	GameShadowCamera(const char* name) :GameObject(name) {}
	void Start()override;
	void Update()override;
private:
	Player* _Player;
	//Vector3 _Dist;	//プレイヤーとの距離
};