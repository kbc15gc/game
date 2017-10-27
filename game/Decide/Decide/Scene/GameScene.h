#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject/Player/Player.h"

class GameScene : public Scene
{
public:
	enum class BGM
	{
		WORLD = 0,
		BOSS1,
		MATI1,
		DEAD,
	};
	GameScene() {};
	void Start()override;
	void Update()override;
private:
	//���[���h��BGM��ύX����B
	void ChangeBGM(BGM);
private:
	//�ʏ헬��Ă鉹�y�B
	SoundSource* _WorldBGM;
	SoundSource* _BossBGM;
	SoundSource* _MatiBGM;
	SoundSource* _GameBGM;
	SoundSource* _DeadBGM;
	BGM _BGM;
	Player* _Player;

};