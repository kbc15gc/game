#include"stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateClose.h"

HistoryBookStateClose::HistoryBookStateClose(HistoryBook* historybookstate):
	HistoryBookState(historybookstate)
{

}

HistoryBookStateClose::~HistoryBookStateClose()
{

}

void HistoryBookStateClose::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationNo::AnimationClose, 0.2f, -1);

	//本のコンポーネントを無効化。
	//_HistoryBook->SetEnable(false);
}

void HistoryBookStateClose::Update()
{

}

void HistoryBookStateClose::Exit()
{

}