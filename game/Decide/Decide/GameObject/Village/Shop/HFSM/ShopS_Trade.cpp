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
	const float SELL_RATE = 0.1f;
}


ShopS_Trade::ShopS_Trade(Shop * shop) :IShopState(shop)
{
	//�w�i	
	_TradeWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyWindouw", 8);
	_TradeWindow->SetTexture(LOADTEXTURE("window.png"));
	_TradeWindow->SetSize(Vector2(800, 420));
	_TradeWindow->transform->SetPosition(Vector3(450, 65, 0));
	_TradeWindow->SetPivot(Vector2(0.5f, 0.0f));

	_ParmWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ParmWindow", _TradeWindow->GetPriorty());
	_ParmWindow->SetTexture(LOADTEXTURE("window.png"));
	_ParmWindow->SetSize(Vector2(256, 400));

	_ParmWindow->SetPivot(Vector2(0.5f, 0.0f));
	_ParmWindow->transform->SetParent(_TradeWindow->transform);
	_ParmWindow->transform->SetLocalPosition(Vector3(600, 0, 0));

	_LRButton[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("LButton", _TradeWindow->GetPriorty());
	_LRButton[0]->SetTexture(LOADTEXTURE("Xbox_360/360_LB.png"));
	_LRButton[0]->SetSize(Vector2(90, 90));
	_LRButton[0]->transform->SetParent(_TradeWindow->transform);
	_LRButton[0]->transform->SetLocalPosition(Vector3(-350, 20, 0));

	_LRButton[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("RButton", _TradeWindow->GetPriorty());
	_LRButton[1]->SetTexture(LOADTEXTURE("Xbox_360/360_RB.png"));
	_LRButton[1]->SetSize(Vector2(90, 90));
	_LRButton[1]->transform->SetParent(_TradeWindow->transform);
	_LRButton[1]->transform->SetLocalPosition(Vector3(350, 20, 0));

	//�p�����[�^�̃e�L�X�g�B
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
	_TopText->transform->SetLocalPosition(Vector3(-370, 50, 0));
	_TopText->Initialize(L"����                 ������ ������    �l�i", 35);
	_TopText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	
	_ValueText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ValueText", _TradeWindow->GetPriorty());
	_ValueText->transform->SetParent(_TradeWindow->transform);
	_ValueText->transform->SetLocalPosition(Vector3(372, 340, 0));
	_ValueText->Initialize(L"���v���z      0$", 50);
	_ValueText->SetAnchor(fbText::TextAnchorE::UpperRight);
	_ValueText->SetKerning(false);

	//�E�B���h�E���A�N�e�B�u��
	_TradeWindow->SetActive(false, true);
	_ParmWindow->SetActive(false, true);
}

ShopS_Trade::~ShopS_Trade()
{
	FOR(i, _NameTexts.size())
	{
		INSTANCE(GameObjectManager)->AddRemoveList(_NameTexts[i]);
	}
	_NameTexts.clear();
	_MoneyTexts.clear();
}

void ShopS_Trade::Update()
{
	if (_TradeItemNum > 0)
	{
		//���ʌ���
		_UpdateTradeNum();
		//�A�C�e���I��
		if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonUp, 0.6f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKU, 0.6f))
		{
			_SetIndex((_Select > 0) ? _Select - 1 : _TradeItemNum - 1);
			//�A�C�e���̏��𑗂�B
			_SendItemInfo(_DisplayList->at(_Select));
		}
		else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonDown, 0.6f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKD, 0.6f))
		{
			_SetIndex((_Select + 1) % _TradeItemNum);
			//�A�C�e���̏��𑗂�B
			_SendItemInfo(_DisplayList->at(_Select));
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
	//�V���b�v�̃X�e�[�g�ێ��B
	_SaveState = _Shop->_State;
	//�E�B���h�E���A�N�e�B�u�ɂ���B
	_TradeWindow->SetActive(true, true);
	if (_SaveState == Shop::ShopStateE::Buy)
	{
		_LRButton[0]->SetActive(false, false);
		_LRButton[1]->SetActive(false, false);
	}

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
		if (VPadInput->IsPush(fbEngine::VPad::ButtonLB1))
		{
			_DisplayType = (_DisplayType > 0) ? _DisplayType - 1 : static_cast<int>(Item::ItemCodeE::Max) - 1;
			_UpdateList();
			_LRButton[0]->SetBlendColor(Color(0.7f, 0.7f, 0.7f, 1.0f));
		}
		else if (VPadInput->IsPush(fbEngine::VPad::ButtonRB1))
		{
			_DisplayType = (_DisplayType + 1) % static_cast<int>(Item::ItemCodeE::Max);
			_UpdateList();
			_LRButton[1]->SetBlendColor(Color(0.7f, 0.7f, 0.7f, 1.0f));
		}else
		{
			_LRButton[0]->SetBlendColor(Color::white);
			_LRButton[1]->SetBlendColor(Color::white);
		}
	}
}

void ShopS_Trade::_SetIndex(int idx)
{
	int old = _Select;
	//�I�����Ă���Y�����ݒ�B
	_Select = min(idx, max(0, _TradeItemNum - 1));
	if (_TradeItemNum > 0)
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
	{
		_Cursor->SetActive(false);
		//�������𑗐M�B
		_Shop->SetDescriptionText("");
		//�p�����[�^��\���B
		_ParmText->SetText("");
	}


	//�J�[�\�������炷�B
	const float posx = -(_TradeWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;
	int displayidx = _Select - _MinIdx + 1;
	float posy = _MenuListHeight * displayidx + _MenuListHeight*0.5f + 40;
	_Cursor->transform->SetLocalPosition(posx, posy, 0);
}

void ShopS_Trade::_UpdateTradeNum()
{
	if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonRight,0.4f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKR, 0.4f))
	{
		int maxNum = 99;
		if(_SaveState == Shop::ShopStateE::Sell)
		{
			//�A�C�e�������Օi���ǂ����H
			if ((*_DisplayList)[_Select]->GetInfo()->TypeID == Item::ItemCodeE::Item)
				maxNum = ((ConsumptionItem*)_DisplayList->at(_Select))->GetHoldNum();
			else
				maxNum = 1;
		}
		
		//���Z�B
		AddTradeNum(maxNum);
	}
	else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonLeft,0.4f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKL, 0.4f))
	{
		//���Z�B
		SubTradeNum();
	}
}

void ShopS_Trade::AddTradeNum(int max)
{
	//�ő�l�����̎��̂݉��Z�B
	if (_TradeNum[_Select] < max)
	{
		//�������X�g�ɒǉ��B
		if (_TradeNum[_Select] == 0)
		{
			_TradeList.push_back(_Select);
			//�\�[�g����B
			std::sort(_TradeList.begin(), _TradeList.end());//�����\�[�g
		}
		//�����������Z�B
		_TradeNum[_Select]++;
		//�������e�L�X�g���X�V�B
		_UpdateMoneyText(_Select);

		auto rate = (_SaveState == Shop::ShopStateE::Sell) ? SELL_RATE : 1.0f;
		//���v���z���X�V�B
		_SumValue += (int)(_DisplayList->at(_Select)->GetValue() * rate);
		char sum[256];
		sprintf(sum, "���v���z      %4d$", _SumValue);
		_ValueText->SetText(sum);
	}
}

void ShopS_Trade::SubTradeNum()
{
	//0���傫���Ƃ��̂݌��Z�B
	if (0 < _TradeNum[_Select])
	{
		//�����������Z�B
		_TradeNum[_Select]--;

		//�������X�g����폜�B�B
		if (_TradeNum[_Select] == 0)
			_TradeList.erase(remove(_TradeList.begin(), _TradeList.end(), _Select), _TradeList.end());

		//�������e�L�X�g���X�V�B
		_UpdateMoneyText(_Select);

		auto rate = (_SaveState == Shop::ShopStateE::Sell) ? SELL_RATE : 1.0f;
		//���v���z���X�V�B
		_SumValue -= (int)(_DisplayList->at(_Select)->GetValue() * rate);
		char sum[256];
		sprintf(sum, "���v���z      %4d$", _SumValue);
		_ValueText->SetText(sum);
	}
}

void ShopS_Trade::_CreateMenu()
{
	//�e�L�X�g�����B
	while (_NameTexts.size() <= 100)
	{
		//�C���X�^���X���B
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _TradeWindow->GetPriorty());

		text->SetFontSize(50);
		text->SetAnchor(fbText::TextAnchorE::UpperLeft);
		text->transform->SetParent(_TradeWindow->transform);

		//���X�g�ɒǉ��B
		_NameTexts.push_back(text);
		//�l�i�B
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
	for (auto text : _NameTexts)
	{
		text->SetActive(false, true);
	}
}

void ShopS_Trade::_UpdateList()
{
	//�\�����郊�X�g�擾�B
	_DisplayList = (_SaveState == Shop::ShopStateE::Buy) ?
		&_Shop->_ItemList:
		&INSTANCE(Inventory)->GetInventoryList(static_cast<Item::ItemCodeE>(_DisplayType));
	
	_TradeItemNum = 0;
	for (auto item : *_DisplayList)
	{
		if (item != nullptr)			
			_TradeItemNum++;
	}
	_TradeNum.clear();
	_TradeNum.resize(_TradeItemNum,0);
	_TradeList.clear();

	//���v���z���������B
	_SumValue = 0;
	char sum[256];
	sprintf(sum, "���v���z      %4d$", _SumValue);
	_ValueText->SetText(sum);
	//�e�L�X�g�̕����X�V�B
	_UpdateText();
	//�I�����Ă���Y�����X�V�B
	_SetIndex(_Select);
	//�\������A�C�e�����X�N���[���B
	_ScrollDisplayItem();
}

void ShopS_Trade::_SetMinIndex(int minidx)
{
	_MinIdx = max(0, minidx);
	_ScrollDisplayItem();
}

void ShopS_Trade::_UpdateText()
{
	_MenuListHeight = 50.0f;

	//�e�L�X�g�ݒ�B
	FOR(i, _TradeItemNum)
	{
		//���O�e�L�X�g���X�V�B
		_UpdateNameText(i);
		//�ő�̍�����ێ��B
		_MenuListHeight = max(_MenuListHeight, _NameTexts[i]->GetLength().y);

		//���z�A�������A���������X�V�B
		_UpdateMoneyText(i);
	}
}

void ShopS_Trade::_UpdateNameText(int idx)
{
	//�A�C�e�����擾�B
	auto item = (*_DisplayList)[idx];
	char menu[256];
	sprintf(menu, "%s", item->GetInfo()->Name);
	//���O��ݒ�B
	_NameTexts[idx]->SetText(menu);
}

void ShopS_Trade::_UpdateMoneyText(int idx)
{
	//�A�C�e�����擾�B
	auto item = (*_DisplayList)[idx];

	char info[256];
	if (_SaveState == Shop::ShopStateE::Buy)
	{
		auto Info = item->GetInfo();
		//�����Ă�����@�������@�l�i�B
		sprintf(info, "%2d   %2d %5d$", Inventory::Instance()->GetHoldNum(Info->TypeID, Info->ID), _TradeNum[idx], item->GetValue());
	}
	else if (_SaveState == Shop::ShopStateE::Sell)
	{
		int value = (int)(item->GetValue() * SELL_RATE);
		if (item->GetInfo()->TypeID == Item::ItemCodeE::Item)
			//�����Ă�����@�������@�l�i�B
			sprintf(info, "%2d   %2d %5d$", ((ConsumptionItem*)item)->GetHoldNum(), _TradeNum[idx], value);
		else
			sprintf(info, "%2d %5d$", _TradeNum[idx], value);
	}
	//���z��ݒ�B
	_MoneyTexts[idx]->SetText(info);
}

void ShopS_Trade::_ScrollDisplayItem()
{
	//��荇������x�S�Ĕ�\���ɂ���B
	_CloseMenu();

	//�\���̍ŏ��Y��������J�E���g���\������B
	for (int i = _MinIdx, count = 1; i < _MinIdx + DISPLAY_ITEM_NUM && i < _TradeItemNum; i++, count++)
	{
		_NameTexts[i]->SetActive(true, true);
		float posx = -(_TradeWindow->GetSize().x / 2) + _Cursor->GetSize().x;
		_NameTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * count + 40, 0);
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
			const char* p[] = { "HP: ","MP","ATK:","DEF:","MAT:","MDE:","DEX:","CRT:","LV: " };
			auto max_idx = ARRAY_SIZE(p);
			FOR(idx, max_idx)
			{
				if (idx == 1)
					continue;
				char per = ' ';
				if (0 < idx && idx < max_idx - 1)
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
				sprintf(text, "DEF %4d -> %s%4d</color>\nRES %4d -> %s%4d</color>",
					player->GetParam(param::DEF), _CalcColorCode(armor->GetDef()), player->GetParam(param::DEF) + armor->GetDef(),
					player->GetParam(param::MDE), _CalcColorCode(armor->GetMagicDef()), player->GetParam(param::MDE) + armor->GetMagicDef());
			}
			else
			{
				sprintf(text, "DEF %4d -> %s%4d</color>\nRES %4d -> %s%4d</color>",
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
				sprintf(text, "ATK %4d -> %s%4d</color>\nDEX %4d -> %s%4d</color>\nCRT %4d -> %s%4d</color>",
					player->GetParam(param::ATK), _CalcColorCode(weapon->GetAtk()), player->GetParam(param::ATK) + weapon->GetAtk(),
					player->GetParam(param::DEX), _CalcColorCode(weapon->GetDex()), player->GetParam(param::DEX) + weapon->GetDex(),
					player->GetParam(param::CRT), _CalcColorCode(weapon->GetCrt()), player->GetParam(param::CRT) + weapon->GetCrt());
			}
			else
			{
				sprintf(text, "ATK %4d -> %s%4d</color>\nDEX %4d -> %s%4d</color>\nCRT %4d -> %s%4d</color>",
					player->GetParam(param::ATK) + equipW->GetAtk(), _CalcColorCode(weapon->GetAtk() - equipW->GetAtk()), player->GetParam(param::ATK) + weapon->GetAtk(),
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
	//�e�L�X�g�B
	char msg[256];
	if (_TradeList.size() > 0)
	{
		sprintf(msg, "�S���� %d$ �ɂȂ�܂��B", _SumValue);
	}
	else
	{
		//���ݑI�����Ă���A�C�e���𔄔��B
		auto item = (*_DisplayList)[_Select];
		int maxNum = 99;
		if (_SaveState == Shop::ShopStateE::Sell)
		{
			//�A�C�e�������Օi���ǂ����H
			if (item->GetInfo()->TypeID == Item::ItemCodeE::Item)
				maxNum = ((ConsumptionItem*)item)->GetHoldNum();
			else
				maxNum = 1;
		}

		//���Z�B
		AddTradeNum(maxNum);
		sprintf(msg, "%s�ł��ˁB %d$ �ɂȂ�܂��B", item->GetInfo()->Name, _SumValue);

		//�A�C�e���������I������Ă��Ȃ��Ƃ��̃��b�Z�[�W�B
		//_Shop->SpeakMess(5);
	}

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

void ShopS_Trade::BuyItem()
{
	bool success = false;
	for (int idx : _TradeList)
	{
		//�C���x���g���֒ǉ��B
		if ((*_DisplayList)[idx]->GetInfo()->TypeID == Item::ItemCodeE::Item)
		{
			//�A�C�e����ǉ��B
			success = INSTANCE(Inventory)->AddItem((Item::ItemInfo*)(*_DisplayList)[idx]->GetInfo(), _TradeNum[idx]);
			if (success)
				//�A�C�e���̒l�i�������𕥂��B
				_Shop->Pay((*_DisplayList)[idx]->GetValue() * _TradeNum[idx]);
		}
		else
		{
			FOR(num, _TradeNum[idx])
			{
				//�����i��ǉ��B
				success = (INSTANCE(Inventory)->AddEquipment((*_DisplayList)[idx]->GetInfo(), false) != nullptr);
				if (!success)
					break;
				//�A�C�e���̒l�i�������𕥂��B
				_Shop->Pay((*_DisplayList)[idx]->GetValue());
			}
		}
		if (!success)
			break;
	}

	if (success)
	{
		//�A�C�e�����w�������Ƃ��̃��b�Z�[�W�B
		_Shop->SpeakMess(3);
	}
	else
	{
		//�������������ς��̂Ƃ��̃��b�Z�[�W�B
		_Shop->SpeakMess(2);
	}
	
}

void ShopS_Trade::SellItem()
{
	bool erase = false;
	int offset = 0;
	for (int idx : _TradeList)
	{
		//���������ɂ��ꂽ����⊮�����Y�����B
		auto now = idx - offset;
		auto item = (*_DisplayList)[now];
		auto info = item->GetInfo();

		int value = (int)(item->GetValue() * SELL_RATE);
		//�b�菈���A�ǂ��������v��������ύX���āB
		if (info->TypeID==Item::ItemCodeE::Item) {
			//�C���x���g������r���B
			if (INSTANCE(Inventory)->SubHoldNum(item, _TradeNum[now]) == true)
			{
				//�A�C�e���̒l�i��������Ⴄ�B
				_Shop->Pay(-value * _TradeNum[now]);
				//�A�C�e���𔃂�������Ƃ��̃��b�Z�[�W�B
				_Shop->SpeakMess(4);
				if (INSTANCE(Inventory)->GetHoldNum(info->TypeID, info->ID) == 0)
				{
					erase = true;
					//�A�C�e�����������̂ł��炷�B
					offset++;
				}
			}
		}
		else
		{
			//�C���x���g������r���B
			if (INSTANCE(Inventory)->SubHoldNum(item, _TradeNum[now]) == true)
			{
				//�A�C�e���̒l�i��������Ⴄ�B
				_Shop->Pay(-value);
				//�A�C�e���𔃂�������Ƃ��̃��b�Z�[�W�B
				_Shop->SpeakMess(4);
				if (INSTANCE(Inventory)->GetHoldNum(info->TypeID, info->ID) == 0)
				{
					erase = true;
					//�A�C�e�����������̂ł��炷�B
					offset++;
				}
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
