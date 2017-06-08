#pragma once
#include "HistoryBookState.h"

//—ğj‘‚ªŠJ‚¢‚Ä‚¢‚éó‘ÔB
class HistoryBookStateOpening :public HistoryBookState
{
public:
	HistoryBookStateOpening(HistoryBook* historybook);
	~HistoryBookStateOpening();

	void Entry()override;

	void Update()override;

	void Exit()override;
};