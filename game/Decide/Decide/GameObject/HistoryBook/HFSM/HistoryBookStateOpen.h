#pragma once
#include "HistoryBookState.h"

//—ğj‘‚ªŠJ‚¢‚½ó‘ÔB
class HistoryBookStateOpen :public HistoryBookState
{
public:
	HistoryBookStateOpen(HistoryBook* historybook);
	~HistoryBookStateOpen();

	void Entry()override;

	void Update()override;

	void Exit()override;
};

