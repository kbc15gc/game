/**
* 歴史書の状態の基底クラスの定義.
*/
#pragma once

/**
* 歴史書クラス.
*/
class HistoryBook;

/**
* 歴史書の状態の基底クラス.
*/
class IHistoryBookState
{
public:

	/**
	* コンストラクタ.
	*/
	IHistoryBookState(HistoryBook* historybook)
	{
		this->_HistoryBook = historybook;
	}

	/**
	* デストラクタ.
	*/
	virtual ~IHistoryBookState()
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

	/** 歴史書のポインタ. */
	HistoryBook* _HistoryBook = nullptr;

};