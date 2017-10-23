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

	/*
	* �`��.
	*/
	void Render()override;
	
	/**
	* �`�b�vID��ݒ�.
	*/
	void SetChipID(ChipID chipID);

	/**
	* �G�h���b�v�̃`�b�vID��ݒ�.
	*/
	void SetDropChipID(ChipID chipID, const Vector3& pos);

private:

	/** �`�b�vID. */
	ChipID _ChipID;

	/** �v���C���[�̃|�C���^. */
	Player*	_Player = nullptr;

	/** �T�E���h�̃|�C���^. */
	SoundSource* _SE = nullptr;

	//���f��
	SkinModel* _Model = nullptr;
	//�}�e���A��
	Material* _Material = nullptr;

};