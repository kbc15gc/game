#pragma once
#include "fbEngine/GameObject.h"
//�Q�[���Ŏg���e����邽�߂̃J����
class GameShadowCamera :public GameObject
{
public:
	GameShadowCamera(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
};