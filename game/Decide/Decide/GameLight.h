#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
//�Q�[�����C�g
class GameLight :public GameObject
{
public:
	GameLight(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
};