#pragma once

class HistoryBook;

//���N���X�B
//���j���̃X�e�[�g�B
class HistoryBookState
{
public:
	HistoryBookState(HistoryBook* historybook)
	{
		this->_HistoryBook = historybook;
	}

	virtual ~HistoryBookState()
	{
	}

	//�X�e�[�g��؂�ւ������ɌĂ΂��B
	virtual void Entry() = 0;

	//�X�V�B
	virtual void Update() = 0;

	//���̃X�e�[�g�ɐ؂�ւ��O�ɌĂ΂��B�B
	virtual void Exit() = 0;
protected:
	HistoryBook* _HistoryBook = nullptr;	//���j���̃R���|�[�l���g�B

};