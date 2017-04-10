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
	//�J�E���g�_�E�����I�������Ȃ�true��Ԃ��B
	bool _CountDown();
private:
	int _LastTime;	//�O�̎���
	double _CountDownTimer;
};