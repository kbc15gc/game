/**
* ���j���̖��g�p��ԃN���X�̒�`.
*/
#pragma once

#include "HistoryBookState.h"

/**
* ���j���̖��g�p��ԃN���X.
*/
class HistoryBookStateUnused : public IHistoryBookState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryBookStateUnused(HistoryBook* historybook) :
		IHistoryBookState(historybook)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryBookStateUnused()
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
