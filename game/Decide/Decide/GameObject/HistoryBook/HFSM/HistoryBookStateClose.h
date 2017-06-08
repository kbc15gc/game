#pragma once
#include "HistoryBookState.h"

//—ğj‘‚ª•Â‚¶‚Ä‚¢‚éó‘ÔB
class HistoryBookStateClose:public HistoryBookState
{
public:
	HistoryBookStateClose(HistoryBook* historybook);

	~HistoryBookStateClose();

	void Entry()override;

	void Update()override;

	void Exit()override;
};
