#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class TitleScene : public Scene
{
public:
	TitleScene() {};
	void Start()override;
	void Update()override;

	void Alpha();
private:
	SoundSource* _TitleBGM;
	ImageObject* _Start;
	Color		_StartColor;
	float		_StartAlpha;
};