#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
//�Q�[���Ŏg���e����邽�߂̃J����
class GameShadowCamera :public GameObject
{
public:
	GameShadowCamera(const char* name) :GameObject(name) {}
	void Start()override;
	void Update()override;
private:
	Player* _Player;
	//Vector3 _Dist;	//�v���C���[�Ƃ̋���
};