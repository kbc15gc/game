#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class GameScene : public Scene
{
public:
	GameScene() {};
	void Start()override;
	void Update()override;
private:
	//�ʏ헬��Ă鉹�y�B
	SoundSource* _WorldSE;
};