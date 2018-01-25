#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include"GameObject\Player\Player.h"

//�Q�[�����C�g
class GameLight :public GameObject
{
public:
	GameLight(const char* name) :GameObject(name) {}
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	void Update()override;

private:

	/** �v���C���[�̃|�C���^. */
	Player* _Player = nullptr;

	/** ���C�g. */
	Light* _Light = nullptr;

};