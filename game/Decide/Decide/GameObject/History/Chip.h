/**
* �`�b�v�N���X�̒�`.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "HistoryInfo.h"

class Player;
class HistoryMenuSelect;

/**
* �`�b�v�N���X.
* ���j�̂�����.
*/
class Chip : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	Chip(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Chip()
	{
	}

	/**
	* �R���X�g���N�^��̏�����.
	*/
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	/**
	* �X�V.
	*/
	void Update()override;
	
	/**
	* �`�b�vID��ݒ�.
	*/
	void SetChipID(ChipID chipID);

private:

	/** �`�b�vID. */
	ChipID _ChipID;

	/** �v���C���[�̃|�C���^. */
	Player*	_Player = nullptr;

	/** �T�E���h�̃|�C���^. */
	SoundSource* _SE = nullptr;

};