/**
* 歴史書のページの状態の基底クラスの定義.
*/
#pragma once

/** 歴史書のページクラス. */
class HistoryPage;

/**
* 歴史書のページの状態の基底クラス.
*/
class IHistoryPageState : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	IHistoryPageState(HistoryPage* historyPage)
	{
		_HistoryPage = historyPage;
	}

	/**
	* デストラクタ.
	*/
	virtual ~IHistoryPageState()
	{
	}

	/**
	* 状態に切り替えたとき呼ばれる.
	*/
	virtual void Entry() = 0;

	/**
	* 更新.
	*/
	virtual void Update() = 0;

	/**
	* 他の状態に変わるとき呼ばれる.
	*/
	virtual void Exit() = 0;

protected:

	/** 歴史書のページクラスのポインタ. */
	HistoryPage* _HistoryPage = nullptr;

};