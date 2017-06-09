#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\Village\HistoryInfo.h"

class Player;
class HistoryMenuSelect;

class Chips : public GameObject
{
public:
	Chips(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;
	
	void SetChipID(const ChipID& id);
private:
	//�v���C���[�I�u�W�F�N�g
	Player*		_Player = nullptr;
	//�q�X�g���[���j���[�Z���N�g�I�u�W�F�N�g
	HistoryMenuSelect* _HistoryMenuSelect = nullptr;
	//�T�E���h
	SoundSource* _SE = nullptr;
	//������ID
	ChipID _ChipID;
};