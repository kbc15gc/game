/**
* ���j���̏�Ԃ̊��N���X�̒�`.
*/
#pragma once

/**
* ���j���N���X.
*/
class HistoryBook;

/**
* ���j���̏�Ԃ̊��N���X.
*/
class IHistoryBookState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	IHistoryBookState(HistoryBook* historybook)
	{
		this->_HistoryBook = historybook;
	}

	/**
	* �f�X�g���N�^.
	*/
	virtual ~IHistoryBookState()
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

	/** ���j���̃|�C���^. */
	HistoryBook* _HistoryBook = nullptr;

};