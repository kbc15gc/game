#pragma once
#include "HistoryBookState.h"

//歴史書が開いた状態。
class HistoryBookStateOpen :public HistoryBookState
{
public:
	HistoryBookStateOpen(HistoryBook* historybook);
	~HistoryBookStateOpen();

	void Entry()override;

	void Update()override;

	void Exit()override;
};

