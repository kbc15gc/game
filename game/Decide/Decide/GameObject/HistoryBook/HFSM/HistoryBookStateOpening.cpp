#include "stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateOpening.h"


HistoryBookStateOpening::HistoryBookStateOpening(HistoryBook* historybook):
	HistoryBookState(historybook)
{

}

HistoryBookStateOpening::~HistoryBookStateOpening()
{

}

void HistoryBookStateOpening::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationNo::AnimationOpening, 0.2f, 1);

	//�{�̃R���|�[�l���g��L�����B
	_HistoryBook->SetEnable(true);

}

void HistoryBookStateOpening::Update()
{
	//�{���J����������{���J���Ă����ԂŌŒ�B
	if (_HistoryBook->GetIsPlay() == false)
	{
		//�J������A�j���[�V�����̍Đ����I�������J������ԂɕύX�B
		_HistoryBook->ChangeState(HistoryBook::State::Open);
	}
}

void HistoryBookStateOpening::Exit()
{

}