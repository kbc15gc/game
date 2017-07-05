/**
* ���j���̃y�[�W�̋��ޏ�ԃN���X�̒�`.
*/
#pragma once

#include"HistoryPageState.h"

/**
* ���j���̃y�[�W�̋��ޏ�ԃN���X.
*/
class HistoryPageStatePutIn : public IHistoryPageState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryPageStatePutIn(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryPageStatePutIn()
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
	Vector3 _InitialPosition = Vector3(0.0f, 0.0f, 3.0f);
	/** �ړ���. */
	Vector3 _MovePosition = Vector3(0.0f, 0.0f, 0.2f);

	/** �ړ����x. */
	float _MoveSpeed = 1.0f;

	/** ��ԃ��[�g. */
	float _LerpRate = 0;

};