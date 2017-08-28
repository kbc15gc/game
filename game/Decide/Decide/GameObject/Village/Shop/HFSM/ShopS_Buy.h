#pragma once
#include "ShopState.h"

class ImageObject;
class TextObject;

//ショップの購入画面のクラス。
class ShopS_Buy :public IShopState
{
public:
	//コンストラクタ。
	//[in] ショップのポインタ。
	ShopS_Buy(Shop* shop);

	//デストラクタ。
	~ShopS_Buy() {}

	//毎フレーム呼ばれる更新処理。
	void Update();

	//状態に切り替えたとき呼ばれる処理。
	void EnterState();

	//現在の状態から抜ける時に呼ばれる処理。
	void ExitState();

	//子に潜る際に呼び出される処理。
	void DiveState();
private:
	void _CreateMenu();
	void _CloseMenu();
	//カーソルを移動させる
	//[in] メニューの何番目に移動するか？
	void MoveMenuCursor(int idx);
private:
	//現在選択している項目
	int idx = 0;
	//カーソルの画像。
	ImageObject* _Cursor;
	//購入ウィンドウの画像。
	ImageObject* _BuyWindow;
	//商品を視覚化したテキストのリスト。
	vector<TextObject*> _MenuTexts;
	//メニューの一項目の縦幅。
	float _MenuListHeight;
};