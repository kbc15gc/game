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
		auto items = _Shop->_ItemList;
		FOR(i, items.size())
		{
			//インスタンス化。
			TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

			//テキスト設定。
			char t[256];
			sprintf(t, "名前:%s,  値段:%d$,  攻撃力:%d", items[i]->Name, items[i]->Value, items[i]->AtkBuff);
			text->SetString(t);
			text->SetFontSize(50);
			text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);

			//高さ設定。
			height += text->GetLength().y;
			_MenuListHeight = max(_MenuListHeight, text->GetLength().y);
			//左端+アイコンの横幅。
			float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x;
			text->transform->SetLocalPosition(posx, height, 0);
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

void ShopS_Buy::MoveMenuCursor(int idx)
{
}
