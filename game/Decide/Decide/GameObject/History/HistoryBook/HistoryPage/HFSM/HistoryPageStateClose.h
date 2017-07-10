/**
* ���j���̃y�[�W�̕����ԃN���X�̒�`.
*/
#pragma once

#include"HistoryPageState.h"

/**
* ���j���̃y�[�W�̕����ԃN���X.
*/
class HistoryPageStateClose : public IHistoryPageState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryPageStateClose(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryPageStateClose()
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

	/** ��]�O�̊p�x. */
	float _BefAngle = 0.0f;
	/** �ړI�̊p�x. */
	float _Angle = 90.0f;

	/** �ړ����x. */
	float _MoveSpeed = 1.0f;

	/** ��ԃ��[�g. */
	float _LerpRate = 0;

};