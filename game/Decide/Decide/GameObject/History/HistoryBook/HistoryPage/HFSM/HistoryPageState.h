/**
* ���j���̃y�[�W�̏�Ԃ̊��N���X�̒�`.
*/
#pragma once

/** ���j���̃y�[�W�N���X. */
class HistoryPage;

/**
* ���j���̃y�[�W�̏�Ԃ̊��N���X.
*/
class IHistoryPageState : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
	*/
	IHistoryPageState(HistoryPage* historyPage)
	{
		_HistoryPage = historyPage;
	}

	/**
	* �f�X�g���N�^.
	*/
	virtual ~IHistoryPageState()
	{
	}

	/**
	* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
	*/
	virtual void Entry() = 0;

	/**
	* �X�V.
	*/
	virtual void Update() = 0;

	/**
	* ���̏�Ԃɕς��Ƃ��Ă΂��.
	*/
	virtual void Exit() = 0;

protected:

	/** ���j���̃y�[�W�N���X�̃|�C���^. */
	HistoryPage* _HistoryPage = nullptr;

};