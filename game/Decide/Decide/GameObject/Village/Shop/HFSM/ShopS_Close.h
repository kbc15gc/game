#pragma once
#include "ShopState.h"

//ショップの販売画面のクラス。
class ShopS_Close :public IShopState
{
public:
	//コンストラクタ。
	//[in] ショップのポインタ。
	ShopS_Close(Shop* shop) :IShopState(shop) {}

	//デストラクタ。
	~ShopS_Close() {}

	//毎フレーム呼ばれる更新処理。
	void Update();

	//状態に切り替えたとき呼ばれる処理。
	void EnterState();

	//現在の状態から抜ける時に呼ばれる処理。
	void ExitState();

	//子に潜る際に呼び出される処理。
	void DiveState();
};