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

};