/**
* 歴史書が開く状態クラスの定義.
*/
#pragma once
#include "HistoryBookState.h"

/**
* 歴史書が開く状態クラス.
*/
class HistoryBookStateOpen :public IHistoryBookState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryBookStateOpen(HistoryBook* historybook) :
		IHistoryBookState(historybook)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryBookStateOpen()
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

};