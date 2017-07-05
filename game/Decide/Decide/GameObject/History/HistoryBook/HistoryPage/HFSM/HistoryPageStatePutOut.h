/**
* 歴史書のページの抜く状態クラスの定義.
*/
#pragma once

#include"HistoryPageState.h"

/**
* 歴史書のページの抜く状態クラス.
*/
class HistoryPageStatePutOut : public IHistoryPageState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryPageStatePutOut(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryPageStatePutOut()
	{
	}

	/**
	* 状態に切り替えたとき呼ばれる.
	*/
	void Entry()override;

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* 他の状態に変わるとき呼ばれる.
	*/
	void Exit()override;

private:

};