#include "stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateCloseing.h"


HistoryBookStateCloseing::HistoryBookStateCloseing(HistoryBook* historybook):
	HistoryBookState(historybook)
{
}


HistoryBookStateCloseing::~HistoryBookStateCloseing()
{
}


void HistoryBookStateCloseing::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationNo::AnimationCloseing, 0.2f, 1);
}

void HistoryBookStateCloseing::Update()
{
	//�{����I������������ԂɕύX�B
	if (_HistoryBook->GetIsPlay() == false)
	{
		//���j���̏�Ԃ������ԂɕύX�B
		_HistoryBook->ChangeState(HistoryBook::State::Close);
	}
		
}

void HistoryBookStateCloseing::Exit()
{
	
}