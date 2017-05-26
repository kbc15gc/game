#pragma once
#include "HistoryBookState.h"

//歴史書を閉じている状態。
class HistoryBookStateCloseing :public HistoryBookState
{
public:
	HistoryBookStateCloseing(HistoryBook* historybook);
	~HistoryBookStateCloseing();

	void Entry()override;

	void Update()override;

	void Exit()override;
};

