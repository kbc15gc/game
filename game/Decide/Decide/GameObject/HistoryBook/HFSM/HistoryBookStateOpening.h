#pragma once
#include "HistoryBookState.h"

//���j�����J���Ă����ԁB
class HistoryBookStateOpening :public HistoryBookState
{
public:
	HistoryBookStateOpening(HistoryBook* historybook);
	~HistoryBookStateOpening();

	void Entry()override;

	void Update()override;

	void Exit()override;
};