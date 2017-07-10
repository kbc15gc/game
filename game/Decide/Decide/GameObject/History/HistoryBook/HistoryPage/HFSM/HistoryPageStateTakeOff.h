/**
* 歴史書から抜いたページの状態クラスの定義.
*/
#pragma once
#include"HistoryPageState.h"

class HistoryPageStateTakeOff : public IHistoryPageState
{
public:
	/**
	* コンストラクタ.
	*/
	HistoryPageStateTakeOff(HistoryPage* historyPage) :
		IHistoryPageState(historyPage)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryPageStateTakeOff()
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

	/** 初期値. */
	Vector3 _InitialPosition = Vector3(0.0f, 0.0f, 0.2f);
	/** 移動先. */
	Vector3 _MovePosition = Vector3(0.0f, 0.0f, 3.0f);

	/** 移動速度. */
	float _MoveSpeed = 1.0f;

	/** 補間レート. */
	float _LerpRate = 0;
};