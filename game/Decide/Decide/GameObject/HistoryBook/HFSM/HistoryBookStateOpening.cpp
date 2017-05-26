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

	//本のコンポーネントを有効化。
	_HistoryBook->SetEnable(true);

}

void HistoryBookStateOpening::Update()
{
	//本を開ききったら本を開いている状態で固定。
	if (_HistoryBook->GetIsPlay() == false)
	{
		//開ききるアニメーションの再生が終わったら開いた状態に変更。
		_HistoryBook->ChangeState(HistoryBook::State::Open);
	}
}

void HistoryBookStateOpening::Exit()
{

}