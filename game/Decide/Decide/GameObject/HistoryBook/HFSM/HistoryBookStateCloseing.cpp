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
	//本を閉じ終わったら閉じた状態に変更。
	if (_HistoryBook->GetIsPlay() == false)
	{
		//歴史書の状態を閉じた状態に変更。
		_HistoryBook->ChangeState(HistoryBook::State::Close);
	}
		
}

void HistoryBookStateCloseing::Exit()
{
	
}