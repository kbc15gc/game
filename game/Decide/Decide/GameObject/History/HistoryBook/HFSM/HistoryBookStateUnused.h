/**
* 歴史書の未使用状態クラスの定義.
*/
#pragma once

#include "HistoryBookState.h"
#include "GameObject\Camera\PlayerCamera.h"

class Player;

/**
* 歴史書の未使用状態クラス.
*/
class HistoryBookStateUnused : public IHistoryBookState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryBookStateUnused(HistoryBook* historybook);

	/**
	* デストラクタ.
	*/
	~HistoryBookStateUnused()
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

	/** ゲームカメラクラスのポインタ. */
	PlayerCamera* _PlayerCamera = nullptr;

	/** プレイヤークラスのポインタ. */
	Player* _Player = nullptr;
	/** プレイヤーの正面方向. */
	Vector3 _PlayerFoward = Vector3::zero;

};
