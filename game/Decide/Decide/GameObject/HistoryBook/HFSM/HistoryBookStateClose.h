#pragma once
#include "HistoryBookState.h"

//���j�������Ă����ԁB
class HistoryBookStateClose:public HistoryBookState
{
public:
	HistoryBookStateClose(HistoryBook* historybook);

	~HistoryBookStateClose();

	void Entry()override;

	void Update()override;

	void Exit()override;
};
