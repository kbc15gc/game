#pragma once
#include "HistoryBookState.h"

//歴史書が閉じている状態。
class HistoryBookStateClose:public HistoryBookState
{
public:
	HistoryBookStateClose(HistoryBook* historybook);

	~HistoryBookStateClose();

	void Entry()override;

	void Update()override;

	void Exit()override;
};
