#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class TitleScene : public Scene
{
public:

	enum Select
	{
		Continue = 0,
		NewGame,
	};

	TitleScene() {};
	void Start()override;
	void Update()override;

	void Alpha();
private:
	SoundSource* _TitleBGM;
	ImageObject* _Start;
	ImageObject* _StartBar;
	ImageObject* _NewGame;
	ImageObject* _Continue;
	Color		_StartColor;
	float		_StartAlpha;
	SoundSource* _StartSE;

	bool _AnyButton;

	Select _Select;
};