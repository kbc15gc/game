#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class TitleScene : public Scene
{
public:
	TitleScene() {};
	void Start()override;
	void Update()override;
private:
	SoundSource* _TitleBGM;
};