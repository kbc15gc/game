#pragma once
#include "HistoryBookState.h"

//���j������Ă����ԁB
class HistoryBookStateCloseing :public HistoryBookState
{
public:
	HistoryBookStateCloseing(HistoryBook* historybook);
	~HistoryBookStateCloseing();

	void Entry()override;

	void Update()override;

	void Exit()override;
};

