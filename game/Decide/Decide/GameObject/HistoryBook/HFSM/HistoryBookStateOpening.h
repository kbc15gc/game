#pragma once
#include "HistoryBookState.h"

//歴史書が開いている状態。
class HistoryBookStateOpening :public HistoryBookState
{
public:
	HistoryBookStateOpening(HistoryBook* historybook);
	~HistoryBookStateOpening();

	void Entry()override;

	void Update()override;

	void Exit()override;
};