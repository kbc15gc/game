/**
* ���j���̃y�[�W�̔�����ԃN���X�̒�`.
*/
#pragma once

#include"HistoryPageState.h"

/**
* ���j���̃y�[�W�̔�����ԃN���X.
*/
class HistoryPageStatePutOut : public IHistoryPageState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryPageStatePutOut(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryPageStatePutOut()
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