/**
* 歴史書のページの捲る状態クラスの定義.
*/
#pragma once

#include"HistoryPageState.h"

/**
* 歴史書のページの捲る状態クラス.
*/
class HistoryPageStateTurn : public IHistoryPageState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryPageStateTurn(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryPageStateTurn()
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