/**
* ���j�����J����ԃN���X�̒�`.
*/
#pragma once
#include "HistoryBookState.h"

/**
* ���j�����J����ԃN���X.
*/
class HistoryBookStateOpen :public IHistoryBookState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryBookStateOpen(HistoryBook* historybook) :
		IHistoryBookState(historybook)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryBookStateOpen()
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

};