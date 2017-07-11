/**
* ���j�����甲�����y�[�W�̏�ԃN���X�̒�`.
*/
#pragma once
#include"HistoryPageState.h"

class HistoryPageStateTakeOff : public IHistoryPageState
{
public:
	/**
	* �R���X�g���N�^.
	*/
	HistoryPageStateTakeOff(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryPageStateTakeOff()
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

	/** �����l. */
	Vector3 _InitialPosition = Vector3(0.0f, 0.0f, 0.2f);
	/** �ړ���. */
	Vector3 _MovePosition = Vector3(0.0f, 0.0f, 3.0f);

	/** �ړ����x. */
	float _MoveSpeed = 1.0f;

	/** ��ԃ��[�g. */
	float _LerpRate = 0;
};