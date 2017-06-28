/**
* 歴史書の待機状態クラスの定義.
*/
#pragma once

#include "HistoryBookState.h"

//#include"GameCamera.h"
#include "GameObject\Camera\PlayerCamera.h"

/**
* 歴史書の待機状態クラス.
*/
class HistoryBookStateIdol : public IHistoryBookState
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryBookStateIdol(HistoryBook* historybook);

	/**
	* デストラクタ.
	*/
	~HistoryBookStateIdol()
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
	PlayerCamera* _PlayerCamera = nullptr;
	/** プレイヤーのポインタ. */
	Player* _Player = nullptr;

};
