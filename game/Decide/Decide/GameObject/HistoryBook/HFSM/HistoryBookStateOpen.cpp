#include "stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateOpen.h"


HistoryBookStateOpen::HistoryBookStateOpen(HistoryBook* historybook):
	HistoryBookState(historybook)
{

}


HistoryBookStateOpen::~HistoryBookStateOpen()
{
	
}

void HistoryBookStateOpen::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationNo::AnimationOpen, 0.2f, 1);
}

void HistoryBookStateOpen::Update()
{

}

void HistoryBookStateOpen::Exit()
{

}