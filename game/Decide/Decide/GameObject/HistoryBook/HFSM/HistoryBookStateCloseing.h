#pragma once
#include "HistoryBookState.h"

//—ğj‘‚ğ•Â‚¶‚Ä‚¢‚éó‘ÔB
class HistoryBookStateCloseing :public HistoryBookState
{
public:
	HistoryBookStateCloseing(HistoryBook* historybook);
	~HistoryBookStateCloseing();

	void Entry()override;

	void Update()override;

	void Exit()override;
};

