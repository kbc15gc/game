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

ShopS_Trade::~ShopS_Trade()
{
	FOR(i, _MenuTexts.size())
	{
		INSTANCE(GameObjectManager)->AddRemoveList(_MenuTexts[i]);
	}
	_MenuTexts.clear();
	_MoneyTexts.clear();
}

void ShopS_Trade::Update()
{
	if (_DisplayList.size() > 0)
	{
		if ((KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU)))
		{
			SetIndex((_Select > 0) ? _Select - 1 : _DisplayList.size() - 1);
		}
		else if ((KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD)))
		{
			SetIndex((_Select + 1) % _DisplayList.size());
		}

		if ((KeyBoardInput->isPush(DIK_RIGHT) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKR)))
		{
			int maxNum = (_SaveState == Shop::ShopStateE::Buy) ? 99 : _DisplayList.at(_Select)->GetHoldNum();
				_SelectNum = min(maxNum, _SelectNum + 1);
		}
		else if ((KeyBoardInput->isPush(DIK_LEFT) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKL)))
		{
			_SelectNum = max(1, _SelectNum - 1);
		}

		//決定(仮)
		if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
		{
			Decision();
		}
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
	_SaveState = _Shop->_State;
	//メニューを作成。
	_CreateMenu();
	//
	SetIndex(_Select);
	//表示更新。
	ScrollDisplayItem();

	_SelectNum = 1;
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
	//テキスト生成。
	while (_MenuTexts.size() <= 30)
	{
		//インスタンス化。
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

		text->SetFontSize(50);
		text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);
		text->transform->SetParent(_BuyWindow->transform);

		//リストに追加。
		_MenuTexts.push_back(text);

		TextObject* money = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

		money->SetFontSize(50);
		money->SetFormat((unsigned int)fbText::TextFormatE::RIGHT);
		money->transform->SetParent(text->transform);

		_MoneyTexts.push_back(money);
	}
	UpdateList();
}

void ShopS_Trade::_CloseMenu()
{
	FOR(i,_MenuTexts.size())
	{
		_MenuTexts[i]->SetActive(false, true);
	}
}

void ShopS_Trade::SetIndex(int idx)
{
	if (_DisplayList.size() > 0)
	{
		//リストの表示更新。
		if (idx >= _MinIdx + DISPLAY_ITEM_NUM)
			SetMinIndex(idx - (DISPLAY_ITEM_NUM - 1));
		else if (idx < _MinIdx)
			SetMinIndex(idx);

		//カーソルをずらす。
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;
		int displayidx = idx - _MinIdx + 1;
		float posy = _MenuListHeight * displayidx + _MenuListHeight*0.5f;
		_Cursor->transform->SetLocalPosition(posx, posy, 0);

		//アイテムの情報を送る。
		if (_Select != idx)
			_Shop->SetDescriptionText(_DisplayList.at(idx)->GetInfo()->Description);

		//選択している添え字設定。
		_Select = idx;

		//選択数初期化。
		_SelectNum = 1;
	}
}

void ShopS_Trade::SetMinIndex(int minidx)
{
	_MinIdx = max(0, minidx);
	ScrollDisplayItem();
}

void ShopS_Trade::ScrollDisplayItem()
{
	//取り合えず一度全て非表示にする。
	_CloseMenu();

	//表示の最小添え字からカウント分表示する。
	for (int i = _MinIdx,count = 1; i < _MinIdx + DISPLAY_ITEM_NUM && i < _DisplayList.size(); i++,count++)
	{
		_MenuTexts[i]->SetActive(true,true);
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x;
		_MenuTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * count, 0);
		_MoneyTexts[i]->transform->SetLocalPosition(_BuyWindow->GetSize().x - 60, 0, 0);
	}
}

void ShopS_Trade::UpdateList()
{
	_DisplayList.clear();
	//表示するリスト取得。
	if (_SaveState == Shop::ShopStateE::Buy)
	{
		_DisplayList = _Shop->_ItemList;
	}
	else if (_SaveState == Shop::ShopStateE::Sell)
	{
		for (int code = 0; code < static_cast<int>(Item::ItemCodeE::Max); code++)
		{
			auto& inventory = INSTANCE(Inventory)->GetInventoryList((Item::ItemCodeE)code);
			for (auto& item : inventory)
			{
				//nullチェック。
				if (item) {
					_DisplayList.push_back(item.get());
				}
			}
		}
	}

	UpdateText();
}

void ShopS_Trade::UpdateText()
{
	//一回閉じる。
	_CloseMenu();

	float height = 0.0f;
	_MenuListHeight = 0.0f;
	//テキスト設定。
	FOR(i, _DisplayList.size())
	{
		_MenuTexts[i]->SetActive(true);
		//テキスト設定。
		char menu[256];
		sprintf(menu, "%s", _DisplayList[i]->GetInfo()->Name);
		_MenuTexts[i]->SetString(menu);

		char info[256];
		if (_SaveState == Shop::ShopStateE::Sell)
			sprintf(info, "%6d$%4d個", _DisplayList[i]->GetInfo()->Value, _DisplayList[i]->GetHoldNum());
		else
			sprintf(info, "%6d$", _DisplayList[i]->GetInfo()->Value);
		_MoneyTexts[i]->SetString(info);
		//高さ設定。
		height += _MenuTexts[i]->GetLength().y;
		//最大の高さを保持。
		_MenuListHeight = max(_MenuListHeight, _MenuTexts[i]->GetLength().y);
	}
}

void ShopS_Trade::Decision()
{
	if (_DisplayList.size() > 0)
	{
		//アイテムを設定。
		_SelectItem = _DisplayList[_Select]->GetInfo();
		//テキスト。
		char msg[256];
		sprintf(msg, "%s を %d 個ですね。\n全部で %d$ になります。", _SelectItem->Name, _SelectNum, _SelectItem->Value*_SelectNum);
		//関数を設定。
		if (_SaveState == Shop::ShopStateE::Buy)
		{
			//お金が足りているか？
			if (INSTANCE(Inventory)->GetPlayerMoney() >= _DisplayList[_Select]->GetInfo()->Value)
			{
				_Shop->_ShopFunc = std::bind(&ShopS_Trade::BuyItem, this);
				//購入確認画面を出す。
				_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
			}
			else
			{
				//購入できない旨を表示。
				sprintf(msg, "お金がたりませんよ。");
			}
		}
		else if (_SaveState == Shop::ShopStateE::Sell)
		{
			_Shop->_ShopFunc = std::bind(&ShopS_Trade::SellItem, this);
			//販売確認画面を出す。
			_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
		}
		_Shop->SetDescriptionText(msg);
	}
}

void ShopS_Trade::BuyItem()
{
	//アイテムの値段分お金を払う。
	INSTANCE(Inventory)->SubtractPlayerMoney(_SelectItem->Value * _SelectNum);
	//インベントリへ追加。
	INSTANCE(Inventory)->AddItem((Item::ItemCodeE)_SelectItem->TypeID, _SelectItem, _SelectNum);
	_Shop->SetDescriptionText("まいどあり。");
}

void ShopS_Trade::SellItem()
{
	//インベントリから排除。
	if (INSTANCE(Inventory)->SubHoldNum(_SelectItem, _SelectNum))
	{
		//削除されていたならリスト更新。
		UpdateList();
	}
	//アイテムの値段分お金を貰う。
	INSTANCE(Inventory)->SubtractPlayerMoney(-_SelectItem->Value * _SelectNum);
}
