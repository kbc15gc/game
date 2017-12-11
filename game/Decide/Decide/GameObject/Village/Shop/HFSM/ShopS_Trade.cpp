#include "stdafx.h"
#include "ShopS_Trade.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\Player\Player.h"

namespace 
{
	//売却時のレート。
	const float SELL_RATE = 0.8f;
}


ShopS_Trade::ShopS_Trade(Shop * shop) :IShopState(shop)
{
	//背景	
	_TradeWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyWindouw", 8);
	_TradeWindow->SetTexture(LOADTEXTURE("window.png"));
	_TradeWindow->SetSize(Vector2(800, 400));
	_TradeWindow->transform->SetPosition(Vector3(450, 50, 0));
	_TradeWindow->SetPivot(Vector2(0.5f, 0.0f));

	_ParmWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ParmWindow", _TradeWindow->GetPriorty());
	_ParmWindow->SetTexture(LOADTEXTURE("window.png"));
	_ParmWindow->SetSize(Vector2(256, 400));

	_ParmWindow->SetPivot(Vector2(0.5f, 0.0f));
	_ParmWindow->transform->SetParent(_TradeWindow->transform);
	_ParmWindow->transform->SetLocalPosition(Vector3(600, 0, 0));
	

	//インスタンス化。
	_ParmText = INSTANCE(GameObjectManager)->AddNew<TextObject>("Parmtext", _ParmWindow->GetPriorty());

	_ParmText->SetFontSize(30);
	_ParmText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	_ParmText->transform->SetParent(_ParmWindow->transform);
	_ParmText->transform->SetLocalPosition(Vector3(-120, 40, 0));
	_ParmText->SetKerning(false);
	_ParmText->SetText("");

	//カーソル
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyCursor", _TradeWindow->GetPriorty());
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->SetSize(Vector2(32, 32));
	_Cursor->transform->SetParent(_TradeWindow->transform);
	_Cursor->transform->SetLocalPosition(Vector3(-10, 10, 0));

	_TopText = INSTANCE(GameObjectManager)->AddNew<TextObject>("TopText", _TradeWindow->GetPriorty());
	_TopText->transform->SetParent(_TradeWindow->transform);
	_TopText->transform->SetLocalPosition(Vector3(-370, 25, 0));
	_TopText->Initialize(L"名称                 所持数 売買個数    値段", 35);
	_TopText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	
	_ValueText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ValueText", _TradeWindow->GetPriorty());
	_ValueText->transform->SetParent(_TradeWindow->transform);
	_ValueText->transform->SetLocalPosition(Vector3(372, 330, 0));
	_ValueText->Initialize(L"合計金額      0$", 50);
	_ValueText->SetAnchor(fbText::TextAnchorE::UpperRight);
	_ValueText->SetKerning(false);

	//ウィンドウを非アクティブに
	_TradeWindow->SetActive(false, true);
	_ParmWindow->SetActive(false, true);
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
	if (_DisplayItemNum > 0)
	{
		//数量決定
		_UpdateTradeNum();
		//アイテム選択
		if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonUp, 0.8f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKU, 0.8f))
		{
			_SetIndex((_Select > 0) ? _Select - 1 : _DisplayItemNum - 1);
		}
		else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonDown, 0.8f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKD, 0.8f))
		{
			_SetIndex((_Select + 1) % _DisplayItemNum);
		}
		//決定(仮)
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
		{
			_Decision();
		}
	}
	//タブ切り替え
	_SwitchTab();
	//キャンセル。
	if (VPadInput->IsPush(fbEngine::VPad::ButtonB))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Select);
	}
}

void ShopS_Trade::EnterState()
{
	//ウィンドウをアクティブにする。
	_TradeWindow->SetActive(true, true);
	//ショップのステート保持。
	_SaveState = _Shop->_State;

	//メニューを作成。
	_CreateMenu();
	//リスト更新。
	_UpdateList();
}

void ShopS_Trade::ExitState()
{
	//
	_SetIndex(0);
	//メニューを閉じる。
	_CloseMenu();
	_TradeWindow->SetActive(false, true);
}

void ShopS_Trade::DiveState()
{
}

void ShopS_Trade::_SwitchTab()
{
	if (_SaveState == Shop::ShopStateE::Sell)
	{
		if (VPadInput->IsPush(fbEngine::VPad::ButtonRB1))
		{
			_DisplayType = (_DisplayType + 1) % static_cast<int>(Item::ItemCodeE::Max);
			_UpdateList();
		}
		else if (VPadInput->IsPush(fbEngine::VPad::ButtonLB1))
		{
			_DisplayType = (_DisplayType > 0) ? _DisplayType - 1 : static_cast<int>(Item::ItemCodeE::Max) - 1;
			_UpdateList();
		}
	}
}

void ShopS_Trade::_UpdateTradeNum()
{
	if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonRight,0.4f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKR, 0.4f))
	{
		int maxNum = 0;
		//アイテムか消耗品かどうか？
		if ((*_DisplayList)[_Select]->GetInfo()->TypeID == Item::ItemCodeE::Item)
			//買うか売るか？
			maxNum = (_SaveState == Shop::ShopStateE::Buy) ? 99 : ((ConsumptionItem*)_DisplayList->at(_Select))->GetHoldNum();
		else
			maxNum = 1;
		
		_TradeNum[_Select] = min(maxNum, _TradeNum[_Select] + 1);
		_UpdateText();
	}
	else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonLeft,0.4f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKL, 0.4f))
	{
		_TradeNum[_Select] = max(0, _TradeNum[_Select] - 1);
		_UpdateText();
	}
}

void ShopS_Trade::_UpdateSelectItem()
{
	_SumValue = 0;
	float rate = (_SaveState == Shop::ShopStateE::Buy) ? 1.0f : SELL_RATE;
	_IndexList.clear();
	FOR(i, _TradeNum.size())
	{
		if (_TradeNum[i] > 0)
		{
			_IndexList.push_back(i);
			//一つ当たりの値段。
			auto val = (*_DisplayList)[i]->GetValue() * rate;
			_SumValue += val * _TradeNum[i];
		}
	}
	char sum[256];
	sprintf(sum, "合計金額      %4d$", _SumValue);
	_ValueText->SetText(sum);
}

void ShopS_Trade::_CreateMenu()
{
	//テキスト生成。
	while (_MenuTexts.size() <= 30)
	{
		//インスタンス化。
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _TradeWindow->GetPriorty());

		text->SetFontSize(50);
		text->SetAnchor(fbText::TextAnchorE::UpperLeft);
		text->transform->SetParent(_TradeWindow->transform);

		//リストに追加。
		_MenuTexts.push_back(text);

		TextObject* money = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _TradeWindow->GetPriorty());

		money->SetFontSize(50);
		money->SetAnchor(fbText::TextAnchorE::UpperRight);
		money->transform->SetParent(text->transform);
		money->SetKerning(false);

		_MoneyTexts.push_back(money);
	}
}

void ShopS_Trade::_CloseMenu()
{
	FOR(i,_MenuTexts.size())
	{
		_MenuTexts[i]->SetActive(false, true);
	}
}

void ShopS_Trade::_UpdateList()
{
	//表示するリスト取得。
	if (_SaveState == Shop::ShopStateE::Buy)
		_DisplayList = &_Shop->_ItemList;
	else if (_SaveState == Shop::ShopStateE::Sell)
		_DisplayList = &INSTANCE(Inventory)->GetInventoryList(static_cast<Item::ItemCodeE>(_DisplayType));
	
	_TradeNum.clear();
	_DisplayItemNum = 0;
	for (int i = 0; i < _DisplayList->size(); i++)
	{
		if (_DisplayList->at(i) != nullptr)
		{
			_DisplayItemNum++;
			_TradeNum.push_back(0);
		}
	}	

	//テキストの文字更新。
	_UpdateText();
	//選択している添え字更新。
	int tmp = _Select;
	_Select = -1;
	_SetIndex(tmp);
	//表示するアイテムをスクロール。
	_ScrollDisplayItem();
}

void ShopS_Trade::_SetIndex(int idx)
{
	int old = _Select;
	//選択している添え字設定。
	_Select = min(idx, max(0, _DisplayItemNum - 1));
	if (_DisplayItemNum > 0)
	{
		if (old != _Select)
			//アイテムの情報を送る。
			_SendItemInfo(_DisplayList->at(_Select));

		//リストの表示更新。
		if (_Select >= _MinIdx + DISPLAY_ITEM_NUM)
			_SetMinIndex(_Select - (DISPLAY_ITEM_NUM - 1));
		else if (_Select < _MinIdx)
			_SetMinIndex(_Select);
		_Cursor->SetActive(true);
	}
	else
		_Cursor->SetActive(false);
	//カーソルをずらす。
	float posx = -(_TradeWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;
	int displayidx = _Select - _MinIdx + 1;
	float posy = _MenuListHeight * displayidx + _MenuListHeight*0.5f + 20;
	_Cursor->transform->SetLocalPosition(posx, posy, 0);
}

void ShopS_Trade::_SetMinIndex(int minidx)
{
	_MinIdx = max(0, minidx);
	_ScrollDisplayItem();
}

void ShopS_Trade::_UpdateText()
{
	float height = 0.0f;
	_MenuListHeight = 0.0f;

	//テキスト設定。
	FOR(i, _DisplayItemNum)
	{
		auto item = (*_DisplayList)[i];
		//テキスト設定。
		char menu[256];
		sprintf(menu, "%s" ,item->GetInfo()->Name);
		_MenuTexts[i]->SetText(menu);

		//高さ設定。
		height += _MenuTexts[i]->GetLength().y;
		//最大の高さを保持。
		_MenuListHeight = max(_MenuListHeight, _MenuTexts[i]->GetLength().y);

		char info[256];

		if (_SaveState == Shop::ShopStateE::Buy)
		{
			auto Info = item->GetInfo();
			//持っている個数　交換個数　値段。
			sprintf(info, "%2d   %2d %5d$", Inventory::Instance()->GetHoldNum(Info->TypeID, Info->ID), _TradeNum[i], (*_DisplayList)[i]->GetValue());
		}
		else if (_SaveState == Shop::ShopStateE::Sell)
		{
			if (item->GetInfo()->TypeID == Item::ItemCodeE::Item)
			{
				//持っている個数　交換個数　値段。
				sprintf(info, "%2d   %2d %5d$", ((ConsumptionItem*)item)->GetHoldNum(), _TradeNum[i], (*_DisplayList)[i]->GetValue());
			}
			else
			{
				sprintf(info, "%2d %5d$", _TradeNum[i], (*_DisplayList)[i]->GetValue());
			}
		}
		
		_MoneyTexts[i]->SetText(info);
	}

	_UpdateSelectItem();
}

void ShopS_Trade::_ScrollDisplayItem()
{
	//取り合えず一度全て非表示にする。
	_CloseMenu();

	//表示の最小添え字からカウント分表示する。
	for (int i = _MinIdx, count = 1; i < _MinIdx + DISPLAY_ITEM_NUM && i < _DisplayItemNum; i++, count++)
	{
		_MenuTexts[i]->SetActive(true, true);
		float posx = -(_TradeWindow->GetSize().x / 2) + _Cursor->GetSize().x;
		_MenuTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * count + 20, 0);
		_MoneyTexts[i]->transform->SetLocalPosition(_TradeWindow->GetSize().x - 60, 0, 0);
	}
}

typedef CharacterParameter::Param param;

void ShopS_Trade::_SendItemInfo(HoldItemBase * item)
{
	try
	{
		//パラメータの文字列を格納。
		char text[512];
		ZeroMemory(text, 512);
		auto code = item->GetInfo()->TypeID;
		if (code == Item::ItemCodeE::Item)
		{
			//消費アイテムにキャスト
			auto citem = dynamic_cast<ConsumptionItem*>(item);
			auto info = static_cast<Item::ItemInfo*>(citem->GetInfo());
			auto& val = info->effectValue;
			
			string tmp = "";
			const char* p[9] = { "HP: ","MP: ","ATK:","DEF:","INT:","RES:","DEX:","CRT:","LV: " };
			FOR(idx,9)
			{
				char per = ' ';
				if (1 < idx && idx < 8)
					per = '%';
				sprintf(text, "%s%s%4d%c</color>\n", p[idx], _CalcColorCode(val[idx]), val[idx], per);
				tmp += text;
			}
			strcpy(text, tmp.c_str());
		}
		else if (code == Item::ItemCodeE::Armor)
		{
			//防具にキャスト。
			auto armor = (HoldArmor*)item;
			//現在の装備取得。
			auto equipA = GetPlayer()->GetEquipment()->armor;
			if (equipA == nullptr)
			{
				sprintf(text, "DEF %4d -> %s%4d</color>\nMDE %4d -> %s%4d</color>",
					player->GetParam(param::DEF), _CalcColorCode(armor->GetDef()), player->GetParam(param::DEF) + armor->GetDef(),
					player->GetParam(param::MDE), _CalcColorCode(armor->GetMagicDef()), player->GetParam(param::MDE) + armor->GetMagicDef());
			}
			else
			{
				sprintf(text, "DEF %4d -> %s%4d</color>\nMDE %4d -> %s%4d</color>",
					player->GetParam(param::DEF) + equipA->GetDef(), _CalcColorCode(armor->GetDef() - equipA->GetDef()), player->GetParam(param::DEF) + armor->GetDef(),
					player->GetParam(param::MDE) + equipA->GetMagicDef(), _CalcColorCode(armor->GetMagicDef() - equipA->GetMagicDef()), player->GetParam(param::MDE) + armor->GetMagicDef());
			}
		}
		else if (code == Item::ItemCodeE::Weapon)
		{
			//武器にキャスト。
			auto weapon = (HoldWeapon*)item;
			//現在の装備取得。
			auto equipW = GetPlayer()->GetEquipment()->weapon;


			if (equipW == nullptr)
			{
				sprintf(text, "ATK %4d -> %s%4d</color>\nMAT %4d -> %s%4d</color>\nDEX %4d -> %s%4d</color>\nCRT %4d -> %s%4d</color>",
					player->GetParam(param::ATK), _CalcColorCode(weapon->GetAtk()), player->GetParam(param::ATK) + weapon->GetAtk(),
					player->GetParam(param::MAT), _CalcColorCode(weapon->GetMagicAtk()), player->GetParam(param::MAT) + weapon->GetMagicAtk(),
					player->GetParam(param::DEX), _CalcColorCode(weapon->GetDex()), player->GetParam(param::DEX) + weapon->GetDex(),
					player->GetParam(param::CRT), _CalcColorCode(weapon->GetCrt()), player->GetParam(param::CRT) + weapon->GetCrt());
			}
			else
			{
				sprintf(text, "ATK %4d -> %s%4d</color>\nMAT %4d -> %s%4d</color>\nDEX %4d -> %s%4d</color>\nCRT %4d -> %s%4d</color>",
					player->GetParam(param::ATK) + equipW->GetAtk(), _CalcColorCode(weapon->GetAtk() - equipW->GetAtk()), player->GetParam(param::ATK) + weapon->GetAtk(),
					player->GetParam(param::MAT) + equipW->GetMagicAtk(), _CalcColorCode(weapon->GetMagicAtk() - equipW->GetMagicAtk()), player->GetParam(param::MAT) + weapon->GetMagicAtk(),
					player->GetParam(param::DEX) + equipW->GetDex(), _CalcColorCode(weapon->GetDex() - equipW->GetDex()), player->GetParam(param::DEX) + weapon->GetDex(),
					player->GetParam(param::CRT) + equipW->GetCrt(), _CalcColorCode(weapon->GetCrt() - equipW->GetCrt()), player->GetParam(param::CRT) + weapon->GetCrt());
			}
		}

		//説明文を送信。
		_Shop->SetDescriptionText(item->GetInfo()->Description);
		//パラメータを表示。
		_ParmText->SetText(text);
	}
	catch (const std::exception&)
	{

	}
}

char * ShopS_Trade::_CalcColorCode(int diff)
{
	//青色。
	if (diff > 0)
		return  "<color=0fffffff>";
	//赤色。
	else if (diff < 0)
		return  "<color=ff0000ff>";
	//白色。
	return "<color=ffffffff>";
}

void ShopS_Trade::_Decision()
{
	
	if (_IndexList.size() > 0)
	{
		//テキスト。
		char msg[256];
		sprintf(msg, "全部で %d$ になります。", _SumValue);
		//関数を設定。
		if (_SaveState == Shop::ShopStateE::Buy)
		{
			//お金が足りているか？
			if (INSTANCE(Inventory)->GetPlayerMoney() >= _SumValue)
			{
				_Shop->SetDescriptionText(msg);
				_Shop->_ShopFunc = std::bind(&ShopS_Trade::BuyItem, this);
				//購入確認画面を出す。
				_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
			}
			else
			{
				//お金が足りないときのメッセージ。
				_Shop->SpeakMess(1);
			}
		}
		else if (_SaveState == Shop::ShopStateE::Sell)
		{
			//
			_Shop->SetDescriptionText(msg);
			_Shop->_ShopFunc = std::bind(&ShopS_Trade::SellItem, this);
			//販売確認画面を出す。
			_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
		}
	}
	else
	{
		//アイテムが何も選択されていないときのメッセージ。
		_Shop->SpeakMess(5);
	}
}

void ShopS_Trade::BuyItem()
{
	for (int idx : _IndexList)
	{
		bool add = true;
		//インベントリへ追加。
		if ((*_DisplayList)[idx]->GetInfo()->TypeID == Item::ItemCodeE::Item)
		{
			//アイテムを追加。
			add = INSTANCE(Inventory)->AddItem((Item::ItemInfo*)(*_DisplayList)[idx]->GetInfo(), _TradeNum[idx]);
		}
		else
		{
			//装備品を追加。
			add = (INSTANCE(Inventory)->AddEquipment((*_DisplayList)[idx]->GetInfo(), false) != nullptr);
		}

		if (add)
		{
			//アイテムの値段分お金を払う。
			_Shop->Pay((*_DisplayList)[idx]->GetValue() * _TradeNum[idx]);
			//アイテムを購入したときのメッセージ。
			_Shop->SpeakMess(3);
		}
		else
		{
			//持ち物がいっぱいのときのメッセージ。
			_Shop->SpeakMess(2);
		}
	}

	
}

void ShopS_Trade::SellItem()
{
	bool erase = false;
	int offset = 0;
	for (int idx : _IndexList)
	{

		//暫定処理、良い処理が思いついたら変更して。
		if ((*_DisplayList)[idx - offset]->GetInfo()->TypeID==Item::ItemCodeE::Item) {
			//インベントリから排除。
			if (INSTANCE(Inventory)->SubHoldNum((*_DisplayList)[idx - offset], _TradeNum[idx - offset])==true)
			{
				erase = true;
				offset++;
			}
			//アイテムの値段分お金を貰う。
			_Shop->Pay(-(*_DisplayList)[idx - offset]->GetValue() * SELL_RATE);
			//アイテムを買い取ったときのメッセージ。
			_Shop->SpeakMess(3);
		}
		else
		{
			//インベントリから排除。
			if (INSTANCE(Inventory)->SubHoldNum((*_DisplayList)[idx - offset], _TradeNum[idx - offset]) == true)
			{
				//アイテムの値段分お金を貰う。
				_Shop->Pay(-(*_DisplayList)[idx - offset]->GetValue() * SELL_RATE);
				//アイテムを買い取ったときのメッセージ。
				_Shop->SpeakMess(3);
				erase = true;
				offset++;
			}
			else
			{
				//装備している武具を売ろうとした。
				_Shop->SpeakMess(6);
			}
		}

	}
	if(erase)
		//削除されていたならリスト更新。
		_UpdateList();
}
