/**
* 歴史書の移動状態クラスの定義.
*/
#pragma once

#include "HistoryBookState.h"

#include"GameCamera.h"

/**
* 歴史書の移動状態クラス.
*/
class HistoryBookStateMove : public IHistoryBookState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryBookStateMove(HistoryBook* historybook);

	/**
	* デストラクタ.
	*/
	~HistoryBookStateMove()
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

	/** ゲームカメラのポインタ. */
	GameCamera* _GameCamera = nullptr;

	/** 補間レート. */
	float _LerpRate = 0.0f;

	/** 現在の位置. */
	Vector3 _Position = Vector3::zero;
	/** 現在の回転. */
	Quaternion _Rotation = Quaternion::Identity;

};
