#pragma once
#include "fbEngine/Scene.h"
class ImageObject;
class TextObject;
class Player;
class SoundSource;

class GameScene : public Scene
{
public:
	GameScene() {};
	void Start()override;
	void Update()override;
	//カウントダウンが終了したならtrueを返す。
	bool _CountDown();
private:
	int _LastTime;	//前の時間
	double _CountDownTimer;
};