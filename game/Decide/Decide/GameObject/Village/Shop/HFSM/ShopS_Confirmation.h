#pragma once
#include "ShopState.h"

class ImageObject;
class TextObject;

//ショップの確認画面のクラス。
class ShopS_Confirmation :public IShopState
{
public:
	//コンストラクタ。
	//[in] ショップのポインタ。
	ShopS_Confirmation(Shop* shop);

	//デストラクタ。
	~ShopS_Confirmation() {}

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
	//確認ウィンドウの画像。
	ImageObject* _ConfirmationWindow;

	//自分の前のステートを保持。
	Shop::ShopStateE _BeforeState;
};