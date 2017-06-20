/**
* 歴史書の閉じる状態クラスの定義.
*/
#pragma once

#include "HistoryBookState.h"

class Player;

/**
* 歴史書の閉じる状態クラス.
*/
class HistoryBookStateClose : public IHistoryBookState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryBookStateClose(HistoryBook* historybook);

	/**
	* デストラクタ.
	*/
	~HistoryBookStateClose()
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

	/** プレイヤークラスのポインタ. */
	Player* _Player = nullptr;
	/** プレイヤーの正面方向. */
	Vector3 _PlayerFoward = Vector3::zero;

};
