#pragma once
#include "ShopState.h"

class ImageObject;
class TextObject;

//ショップの販売画面のクラス。
class ShopS_Sell :public IShopState
{
public:
	//コンストラクタ。
	//[in] ショップのポインタ。
	ShopS_Sell(Shop* shop) :IShopState(shop) {}

	//デストラクタ。
	~ShopS_Sell() {}

	//毎フレーム呼ばれる更新処理。
	void Update();

	//状態に切り替えたとき呼ばれる処理。
	void EnterState();

	//現在の状態から抜ける時に呼ばれる処理。
	void ExitState();

	//子に潜る際に呼び出される処理。
	void DiveState();
private:
	//カーソルの画像。
	ImageObject* _Cursor;

};