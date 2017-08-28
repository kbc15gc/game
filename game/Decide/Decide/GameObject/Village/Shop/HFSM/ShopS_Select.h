#pragma once
#include "ShopState.h"

class ImageObject;
class TextObject;

//ショップの選択画面のクラス。
class ShopS_Select:public IShopState
{
public:
	//コンストラクタ。
	//[in] ショップのポインタ。
	ShopS_Select(Shop* shop);

	//デストラクタ。
	~ShopS_Select() {}

	//毎フレーム呼ばれる更新処理。
	void Update();

	//状態に切り替えたとき呼ばれる処理。
	void EnterState();

	//現在の状態から抜ける時に呼ばれる処理。
	void ExitState();

	//子に潜る際に呼び出される処理。
	void DiveState();
private:
	//選択している場所。
	int select = 0;
	//カーソルの画像。
	ImageObject* _Cursor;
	//選択ウィンドウの画像。
	ImageObject* _SelectWindow;
};