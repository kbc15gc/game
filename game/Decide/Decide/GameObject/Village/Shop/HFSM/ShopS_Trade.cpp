#include "stdafx.h"
#include "ShopS_Trade.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\Player\Player.h"

namespace 
{
	//���p���̃��[�g�B
	const float SELL_RATE = 0.8f;
}


ShopS_Trade::ShopS_Trade(Shop * shop) :IShopState(shop)
{
	//�w�i	
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
	

	//�C���X�^���X���B
	_ParmText = INSTANCE(GameObjectManager)->AddNew<TextObject>("Parmtext", _ParmWindow->GetPriorty());

	_ParmText->SetFontSize(30);
	_ParmText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	_ParmText->transform->SetParent(_ParmWindow->transform);
	_ParmText->transform->SetLocalPosition(Vector3(-120, 40, 0));
	_ParmText->SetKerning(false);
	_ParmText->SetText("");

	//�J�[�\��
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyCursor", _TradeWindow->GetPriorty());
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->SetSize(Vector2(32, 32));
	_Cursor->transform->SetParent(_TradeWindow->transform);
	_Cursor->transform->SetLocalPosition(Vector3(-10, 10, 0));

	_TopText = INSTANCE(GameObjectManager)->AddNew<TextObject>("TopText", _TradeWindow->GetPriorty());
	_TopText->transform->SetParent(_TradeWindow->transform);
	_TopText->transform->SetLocalPosition(Vector3(-370, 25, 0));
	_TopText->Initialize(L"����                 ������ ������    �l�i", 35);
	_TopText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	
	_ValueText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ValueText", _TradeWindow->GetPriorty());
	_ValueText->transform->SetParent(_TradeWindow->transform);
	_ValueText->transform->SetLocalPosition(Vector3(372, 330, 0));
	_ValueText->Initialize(L"���v���z      0$", 50);
	_ValueText->SetAnchor(fbText::TextAnchorE::UpperRight);
	_ValueText->SetKerning(false);

	//�E�B���h�E���A�N�e�B�u��
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
		//���ʌ���
		_UpdateTradeNum();
		//�A�C�e���I��
		if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonUp, 0.8f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKU, 0.8f))
		{
			_SetIndex((_Select > 0) ? _Select - 1 : _DisplayItemNum - 1);
		}
		else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonDown, 0.8f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKD, 0.8f))
		{
			_SetIndex((_Select + 1) % _DisplayItemNum);
		}
		//����(��)
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
		{
			_Decision();
		}
	}
	//�^�u�؂�ւ�
	_SwitchTab();
	//�L�����Z���B
	if (VPadInput->IsPush(fbEngine::VPad::ButtonB))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Select);
	}
}

void ShopS_Trade::EnterState()
{
	//�E�B���h�E���A�N�e�B�u�ɂ���B
	_TradeWindow->SetActive(true, true);
	//�V���b�v�̃X�e�[�g�ێ��B
	_SaveState = _Shop->_State;

	//���j���[���쐬�B
	_CreateMenu();
	//���X�g�X�V�B
	_UpdateList();
}

void ShopS_Trade::ExitState()
{
	//
	_SetIndex(0);
	//���j���[�����B
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
		//�A�C�e�������Օi���ǂ����H
		if ((*_DisplayList)[_Select]->GetInfo()->TypeID == Item::ItemCodeE::Item)
			//���������邩�H
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
			//�������̒l�i�B
			auto val = (*_DisplayList)[i]->GetValue() * rate;
			_SumValue += val * _TradeNum[i];
		}
	}
	char sum[256];
	sprintf(sum, "���v���z      %4d$", _SumValue);
	_ValueText->SetText(sum);
}

void ShopS_Trade::_CreateMenu()
{
	//�e�L�X�g�����B
	while (_MenuTexts.size() <= 30)
	{
		//�C���X�^���X���B
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _TradeWindow->GetPriorty());

		text->SetFontSize(50);
		text->SetAnchor(fbText::TextAnchorE::UpperLeft);
		text->transform->SetParent(_TradeWindow->transform);

		//���X�g�ɒǉ��B
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
	//�\�����郊�X�g�擾�B
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

	//�e�L�X�g�̕����X�V�B
	_UpdateText();
	//�I�����Ă���Y�����X�V�B
	int tmp = _Select;
	_Select = -1;
	_SetIndex(tmp);
	//�\������A�C�e�����X�N���[���B
	_ScrollDisplayItem();
}

void ShopS_Trade::_SetIndex(int idx)
{
	int old = _Select;
	//�I�����Ă���Y�����ݒ�B
	_Select = min(idx, max(0, _DisplayItemNum - 1));
	if (_DisplayItemNum > 0)
	{
		if (old != _Select)
			//�A�C�e���̏��𑗂�B
			_SendItemInfo(_DisplayList->at(_Select));

		//���X�g�̕\���X�V�B
		if (_Select >= _MinIdx + DISPLAY_ITEM_NUM)
			_SetMinIndex(_Select - (DISPLAY_ITEM_NUM - 1));
		else if (_Select < _MinIdx)
			_SetMinIndex(_Select);
		_Cursor->SetActive(true);
	}
	else
		_Cursor->SetActive(false);
	//�J�[�\�������炷�B
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

	//�e�L�X�g�ݒ�B
	FOR(i, _DisplayItemNum)
	{
		auto item = (*_DisplayList)[i];
		//�e�L�X�g�ݒ�B
		char menu[256];
		sprintf(menu, "%s" ,item->GetInfo()->Name);
		_MenuTexts[i]->SetText(menu);

		//�����ݒ�B
		height += _MenuTexts[i]->GetLength().y;
		//�ő�̍�����ێ��B
		_MenuListHeight = max(_MenuListHeight, _MenuTexts[i]->GetLength().y);

		char info[256];

		if (_SaveState == Shop::ShopStateE::Buy)
		{
			auto Info = item->GetInfo();
			//�����Ă�����@�������@�l�i�B
			sprintf(info, "%2d   %2d %5d$", Inventory::Instance()->GetHoldNum(Info->TypeID, Info->ID), _TradeNum[i], (*_DisplayList)[i]->GetValue());
		}
		else if (_SaveState == Shop::ShopStateE::Sell)
		{
			if (item->GetInfo()->TypeID == Item::ItemCodeE::Item)
			{
				//�����Ă�����@�������@�l�i�B
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
	//��荇������x�S�Ĕ�\���ɂ���B
	_CloseMenu();

	//�\���̍ŏ��Y��������J�E���g���\������B
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
		//�p�����[�^�̕�������i�[�B
		char text[512];
		ZeroMemory(text, 512);
		auto code = item->GetInfo()->TypeID;
		if (code == Item::ItemCodeE::Item)
		{
			//����A�C�e���ɃL���X�g
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
			//�h��ɃL���X�g�B
			auto armor = (HoldArmor*)item;
			//���݂̑����擾�B
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
			//����ɃL���X�g�B
			auto weapon = (HoldWeapon*)item;
			//���݂̑����擾�B
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

		//�������𑗐M�B
		_Shop->SetDescriptionText(item->GetInfo()->Description);
		//�p�����[�^��\���B
		_ParmText->SetText(text);
	}
	catch (const std::exception&)
	{

	}
}

char * ShopS_Trade::_CalcColorCode(int diff)
{
	//�F�B
	if (diff > 0)
		return  "<color=0fffffff>";
	//�ԐF�B
	else if (diff < 0)
		return  "<color=ff0000ff>";
	//���F�B
	return "<color=ffffffff>";
}

void ShopS_Trade::_Decision()
{
	
	if (_IndexList.size() > 0)
	{
		//�e�L�X�g�B
		char msg[256];
		sprintf(msg, "�S���� %d$ �ɂȂ�܂��B", _SumValue);
		//�֐���ݒ�B
		if (_SaveState == Shop::ShopStateE::Buy)
		{
			//����������Ă��邩�H
			if (INSTANCE(Inventory)->GetPlayerMoney() >= _SumValue)
			{
				_Shop->SetDescriptionText(msg);
				_Shop->_ShopFunc = std::bind(&ShopS_Trade::BuyItem, this);
				//�w���m�F��ʂ��o���B
				_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
			}
			else
			{
				//����������Ȃ��Ƃ��̃��b�Z�[�W�B
				_Shop->SpeakMess(1);
			}
		}
		else if (_SaveState == Shop::ShopStateE::Sell)
		{
			//
			_Shop->SetDescriptionText(msg);
			_Shop->_ShopFunc = std::bind(&ShopS_Trade::SellItem, this);
			//�̔��m�F��ʂ��o���B
			_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
		}
	}
	else
	{
		//�A�C�e���������I������Ă��Ȃ��Ƃ��̃��b�Z�[�W�B
		_Shop->SpeakMess(5);
	}
}

void ShopS_Trade::BuyItem()
{
	for (int idx : _IndexList)
	{
		bool add = true;
		//�C���x���g���֒ǉ��B
		if ((*_DisplayList)[idx]->GetInfo()->TypeID == Item::ItemCodeE::Item)
		{
			//�A�C�e����ǉ��B
			add = INSTANCE(Inventory)->AddItem((Item::ItemInfo*)(*_DisplayList)[idx]->GetInfo(), _TradeNum[idx]);
		}
		else
		{
			//�����i��ǉ��B
			add = (INSTANCE(Inventory)->AddEquipment((*_DisplayList)[idx]->GetInfo(), false) != nullptr);
		}

		if (add)
		{
			//�A�C�e���̒l�i�������𕥂��B
			_Shop->Pay((*_DisplayList)[idx]->GetValue() * _TradeNum[idx]);
			//�A�C�e�����w�������Ƃ��̃��b�Z�[�W�B
			_Shop->SpeakMess(3);
		}
		else
		{
			//�������������ς��̂Ƃ��̃��b�Z�[�W�B
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

		//�b�菈���A�ǂ��������v��������ύX���āB
		if ((*_DisplayList)[idx - offset]->GetInfo()->TypeID==Item::ItemCodeE::Item) {
			//�C���x���g������r���B
			if (INSTANCE(Inventory)->SubHoldNum((*_DisplayList)[idx - offset], _TradeNum[idx - offset])==true)
			{
				erase = true;
				offset++;
			}
			//�A�C�e���̒l�i��������Ⴄ�B
			_Shop->Pay(-(*_DisplayList)[idx - offset]->GetValue() * SELL_RATE);
			//�A�C�e���𔃂�������Ƃ��̃��b�Z�[�W�B
			_Shop->SpeakMess(3);
		}
		else
		{
			//�C���x���g������r���B
			if (INSTANCE(Inventory)->SubHoldNum((*_DisplayList)[idx - offset], _TradeNum[idx - offset]) == true)
			{
				//�A�C�e���̒l�i��������Ⴄ�B
				_Shop->Pay(-(*_DisplayList)[idx - offset]->GetValue() * SELL_RATE);
				//�A�C�e���𔃂�������Ƃ��̃��b�Z�[�W�B
				_Shop->SpeakMess(3);
				erase = true;
				offset++;
			}
			else
			{
				//�������Ă��镐��𔄂낤�Ƃ����B
				_Shop->SpeakMess(6);
			}
		}

	}
	if(erase)
		//�폜����Ă����Ȃ烊�X�g�X�V�B
		_UpdateList();
}
