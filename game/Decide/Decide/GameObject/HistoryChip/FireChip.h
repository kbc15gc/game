#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class Player;
class HistoryMenuSelect;

class FireChip : public GameObject
{
public:
	FireChip(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	//�v���C���[�I�u�W�F�N�g
	Player*		_Player;
	//�q�X�g���[���j���[�Z���N�g�I�u�W�F�N�g
	HistoryMenuSelect* _HistoryMenuSelect;
	//�T�E���h
	SoundSource* _SE;
};
