#pragma once
#include "HistoryBookState.h"

//���j�����J������ԁB
class HistoryBookStateOpen :public HistoryBookState
{
public:
	HistoryBookStateOpen(HistoryBook* historybook);
	~HistoryBookStateOpen();

	void Entry()override;

	void Update()override;

	void Exit()override;
};

