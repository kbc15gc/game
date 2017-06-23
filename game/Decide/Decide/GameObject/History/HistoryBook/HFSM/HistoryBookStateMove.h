/**
* ���j���̈ړ���ԃN���X�̒�`.
*/
#pragma once

#include "HistoryBookState.h"

#include"GameCamera.h"

/**
* ���j���̈ړ���ԃN���X.
*/
class HistoryBookStateMove : public IHistoryBookState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryBookStateMove(HistoryBook* historybook);

	/**
	* �f�X�g���N�^.
	*/
	~HistoryBookStateMove()
	{
	}

	/**
	* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
	*/
	void Entry()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* ���̏�Ԃɕς��Ƃ��Ă΂��.
	*/
	void Exit()override;

private:

	/** �Q�[���J�����̃|�C���^. */
	GameCamera* _GameCamera = nullptr;

	/** ��ԃ��[�g. */
	float _LerpRate = 0.0f;

	/** ���݂̈ʒu. */
	Vector3 _Position = Vector3::zero;
	/** ���݂̉�]. */
	Quaternion _Rotation = Quaternion::Identity;

};
