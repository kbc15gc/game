#pragma once

class HistoryBook;

//基底クラス。
//歴史書のステート。
class HistoryBookState
{
public:
	HistoryBookState(HistoryBook* historybook)
	{
		this->_HistoryBook = historybook;
	}

	virtual ~HistoryBookState()
	{
	}

	//ステートを切り替えた時に呼ばれる。
	virtual void Entry() = 0;

	//更新。
	virtual void Update() = 0;

	//次のステートに切り替え前に呼ばれる。。
	virtual void Exit() = 0;
protected:
	HistoryBook* _HistoryBook = nullptr;	//歴史書のコンポーネント。

};