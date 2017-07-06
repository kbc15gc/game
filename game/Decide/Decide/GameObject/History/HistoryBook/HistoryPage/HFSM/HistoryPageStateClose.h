/**
* 歴史書のページの閉じる状態クラスの定義.
*/
#pragma once

#include"HistoryPageState.h"

/**
* 歴史書のページの閉じる状態クラス.
*/
class HistoryPageStateClose : public IHistoryPageState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryPageStateClose(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryPageStateClose()
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

	/** 回転前の角度. */
	float _BefAngle = 0.0f;
	/** 目的の角度. */
	float _Angle = 90.0f;

	/** 移動速度. */
	float _MoveSpeed = 1.0f;

	/** 補間レート. */
	float _LerpRate = 0;

};