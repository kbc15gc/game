#include "stdafx.h"
#include "ShopS_Buy.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

ShopS_Buy::ShopS_Buy(Shop * shop) :IShopState(shop)
{
	//背景	
	_BuyWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyWindouw", 8);
	_BuyWindow->SetTexture(LOADTEXTURE("window.png"));
	_BuyWindow->SetSize(Vector2(800, 400));
	_BuyWindow->transform->SetPosition(Vector3(450, 50, 0));
	_BuyWindow->SetPivot(Vector2(0.5f, 0.0f));

	//カーソル
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_BuyWindow->transform);
	_Cursor->transform->SetLocalPosition(Vector3(-10, 10, 0));

	//ウィンドウを非アクティブに
	_BuyWindow->SetActive(false, true);
}

void ShopS_Buy::Update()
{
	//カーソル移動。
	MoveMenuCursor();

	//決定(仮)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//購入確認画面を出す。
		_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
	}
	//キャンセル。
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Select);
	}
}

void ShopS_Buy::EnterState()
{
	idx = 0;
	//メニューを作成。
	_CreateMenu();
	UpdateDisplayItem();
	//ウィンドウをアクティブにする。
	_BuyWindow->SetActive(true, true);
}

void ShopS_Buy::ExitState()
{
	//メニューを閉じる。
	_CloseMenu();
	_BuyWindow->SetActive(false, true);
}

void ShopS_Buy::DiveState()
{
}

void ShopS_Buy::_CreateMenu()
{
	if (_MenuTexts.size() <= 0)
	{
		float height = 0.0f;
		_MenuListHeight = 0.0f;
		//ショップの品ぞろえと同じ量、テキストを生成。
		auto items = _Shop->_ItemList;
		FOR(i, items.size())
		{
			//インスタンス化。
			TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

			//テキスト設定。
			char t[256];
			sprintf(t, "名前:%s,  値段:%d$", items[i]->Name, items[i]->Value);
			text->SetString(t);
			text->SetFontSize(50);
			text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);

			//高さ設定。
			height += text->GetLength().y;
			//最大の高さを保持。
			_MenuListHeight = max(_MenuListHeight, text->GetLength().y);
			//ウィンドウの子に登録。
			text->transform->SetParent(_BuyWindow->transform);

			//リストに追加。
			_MenuTexts.push_back(text);
		}
	}
}

void ShopS_Buy::_CloseMenu()
{
	FOR(i, _MenuTexts.size())
	{
		INSTANCE(GameObjectManager)->AddRemoveList(_MenuTexts[i]);
	}
	_MenuTexts.clear();
}

void ShopS_Buy::MoveMenuCursor()
{
	//項目が変更されたか？
	bool change = false;

	if (change = (KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU)))
	{
		idx = (idx > 0) ? idx - 1 : _Shop->_ItemList.size() - 1;
	}
	else if (change = (KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD)))
	{
		idx = (idx + 1) % _Shop->_ItemList.size();
	}

	//変更されたか？
	if (change)
	{
		//カーソルをずらす。
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;

		float posy = _MenuListHeight * (idx + 1) + _MenuListHeight*0.5f;
		_Cursor->transform->SetLocalPosition(posx, posy, 0);

		//アイテムの情報を送る。
		_Shop->SetDescriptionText(_Shop->_ItemList[idx]->Description);
	}
}

void ShopS_Buy::UpdateDisplayItem()
{
	//取り合えず一度全て非表示にする。
	for each (auto text in _MenuTexts)
	{
		text->SetActive(false);
	}

	//表示の最小添え字からカウント分表示する。
	for (int i = _MinIdx; i < _MinIdx + (DISPLAY_ITEM_NUM-1) && i < _MenuTexts.size(); i++)
	{
		_MenuTexts[i]->SetActive(true);
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x;
		_MenuTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * i, 0);
	}
}
