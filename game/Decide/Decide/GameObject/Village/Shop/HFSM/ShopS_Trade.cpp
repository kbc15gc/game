#include "stdafx.h"
#include "ShopS_Trade.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

ShopS_Trade::ShopS_Trade(Shop * shop) :IShopState(shop)
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

void ShopS_Trade::Update()
{
	if ((KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU)))
	{
		SetIndex((_Select > 0) ? _Select - 1 : _ItemList.size() - 1);
	}
	else if ((KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD)))
	{
		SetIndex((_Select + 1) % _ItemList.size());
	}

	//決定(仮)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		Decision();
	}
	//キャンセル。
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Select);
	}
}

void ShopS_Trade::EnterState()
{
	//ウィンドウをアクティブにする。
	_BuyWindow->SetActive(true, true);
	//メニューを作成。
	_CreateMenu();
	//
	SetIndex(_Select);
	//表示更新。
	UpdateDisplayItem();
}

void ShopS_Trade::ExitState()
{
	//
	SetIndex(0);
	SetMinIndex(0);
	//メニューを閉じる。
	_CloseMenu();
	_BuyWindow->SetActive(false, true);
}

void ShopS_Trade::DiveState()
{
}

void ShopS_Trade::_CreateMenu()
{
	if (_MenuTexts.size() <= 0)
	{
		float height = 0.0f;
		_MenuListHeight = 0.0f;

		//ショップの品ぞろえと同じ量、テキストを生成。
		if (_Shop->_State == Shop::ShopStateE::Buy)
		{
			_ItemList = _Shop->_ItemList;
		}
		else if (_Shop->_State == Shop::ShopStateE::Sell)
		{
			_ItemList = INSTANCE(Inventory)->GetInfoList();
		}
		FOR(i, _ItemList.size())
		{
			//インスタンス化。
			TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

			//テキスト設定。
			char t[256];
			sprintf(t, "名前:%s,  値段:%d$", _ItemList[i]->Name, _ItemList[i]->Value);
			text->SetString(t);
			text->SetFontSize(50);
			text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);
			text->transform->SetParent(_BuyWindow->transform);
			//リストに追加。
			_MenuTexts.push_back(text);

			//高さ設定。
			height += text->GetLength().y;
			//最大の高さを保持。
			_MenuListHeight = max(_MenuListHeight, text->GetLength().y);
		}
	}
}

void ShopS_Trade::_CloseMenu()
{
	FOR(i, _MenuTexts.size())
	{
		INSTANCE(GameObjectManager)->AddRemoveList(_MenuTexts[i]);
	}
	_MenuTexts.clear();
}

void ShopS_Trade::SetIndex(int idx)
{
	//選択している添え字設定。
	_Select = idx;

	//リストの表示更新。
	if (_Select >= _MinIdx + DISPLAY_ITEM_NUM)
		SetMinIndex(_Select - (DISPLAY_ITEM_NUM - 1));
	else if (_Select < _MinIdx)
		SetMinIndex(_Select);

	//カーソルをずらす。
	float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;
	int displayidx = _Select - _MinIdx + 1;
	float posy = _MenuListHeight * displayidx + _MenuListHeight*0.5f;
	_Cursor->transform->SetLocalPosition(posx, posy, 0);

	//アイテムの情報を送る。
	_Shop->SetDescriptionText(_ItemList[_Select]->Description);
}

void ShopS_Trade::SetMinIndex(int minidx)
{
	_MinIdx = max(0, minidx);
	UpdateDisplayItem();
}

void ShopS_Trade::UpdateDisplayItem()
{
	//取り合えず一度全て非表示にする。
	for each (auto text in _MenuTexts)
	{
		text->SetActive(false);
	}

	//表示の最小添え字からカウント分表示する。
	for (int i = _MinIdx,count = 1; i < _MinIdx + DISPLAY_ITEM_NUM && i < _MenuTexts.size(); i++,count++)
	{
		_MenuTexts[i]->SetActive(true);
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x;
		_MenuTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * count, 0);
	}
}

void ShopS_Trade::Decision()
{
	//お金が足りているか？
	if (INSTANCE(Inventory)->GetPlayerMoney() >= _ItemList[_Select]->Value)
	{
		//関数を設定。
		if (_Shop->_State == Shop::ShopStateE::Buy)
		{
			_Shop->_ShopFunc = std::bind(&ShopS_Trade::BuyItem, this, std::placeholders::_1);
			//テキスト設定。
			_Shop->SetDescriptionText("これを買うのかい？");
		}
		else if (_Shop->_State == Shop::ShopStateE::Sell)
		{
			_Shop->_ShopFunc = std::bind(&ShopS_Trade::SellItem, this, std::placeholders::_1);//テキスト設定。
			_Shop->SetDescriptionText("これを売るのかい？");
		}
		//アイテムを設定。
		_Shop->_SelectItem = _ItemList[_Select];
		//購入確認画面を出す。
		_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
	}
	else
	{
		//購入できない旨を表示。
		_Shop->SetDescriptionText("お金が足りねぇ！");
	}
}

void ShopS_Trade::BuyItem(Item::BaseInfo *info)
{
	//アイテムの値段分お金を払う。
	INSTANCE(Inventory)->SubtractPlayerMoney(info->Value);
	//インベントリへ追加。
	INSTANCE(Inventory)->AddItem((ItemManager::ItemCodeE)info->TypeID,info);
}

void ShopS_Trade::SellItem(Item::BaseInfo *info)
{
	//インベントリから排除。
	//INSTANCE(Inventory)->((ItemManager::ItemCodeE)info->TypeID, info.id);
	//アイテムの値段分お金を払う。
	INSTANCE(Inventory)->SubtractPlayerMoney(-info->Value);
}
