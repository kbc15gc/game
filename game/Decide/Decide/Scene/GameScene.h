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
		MATI1
	};
	GameScene() {};
	void Start()override;
	void Update()override;
	void ChangeBGM(BGM);
private:
	//í èÌó¨ÇÍÇƒÇÈâπäyÅB
	SoundSource* _WorldBGM;
	SoundSource* _BossBGM;
	SoundSource* _MatiBGM;
	SoundSource* _GameBGM;
	BGM _BGM;
	Player* _Player;

};