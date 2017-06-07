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
	//’Êí—¬‚ê‚Ä‚é‰¹ŠyB
	SoundSource* _WorldSE;
};