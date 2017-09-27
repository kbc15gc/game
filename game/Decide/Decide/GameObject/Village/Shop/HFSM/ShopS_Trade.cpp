#include "stdafx.h"
#include "ShopS_Trade.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"


ShopS_Trade::ShopS_Trade(Shop * shop) :IShopState(shop)
{
	//�w�i	
	_TradeWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyWindouw", 8);
	_TradeWindow->SetTexture(LOADTEXTURE("window.png"));
	_TradeWindow->SetSize(Vector2(800, 400));
	_TradeWindow->transform->SetPosition(Vector3(450, 50, 0));
	_TradeWindow->SetPivot(Vector2(0.5f, 0.0f));

	_ParmWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ParmWindow", 8);
	_ParmWindow->SetTexture(LOADTEXTURE("window.png"));
	_ParmWindow->SetSize(Vector2(256, 400));
	_ParmWindow->transform->SetLocalPosition(Vector3(600, 0, 0));
	_ParmWindow->SetPivot(Vector2(0.5f, 0.0f));
	_ParmWindow->transform->SetParent(_TradeWindow->transform);

	//�C���X�^���X���B
	_ParmText = INSTANCE(GameObjectManager)->AddNew<TextObject>("Parmtext", _TradeWindow->GetPriorty());

	_ParmText->SetFontSize(30);
	_ParmText->SetAnchor(fbText::TextAnchorE::MiddleLeft);
	_ParmText->transform->SetParent(_ParmWindow->transform);
	_ParmText->transform->SetLocalPosition(Vector3(-120, 40, 0));
	_ParmText->SetKerning(false);
	_ParmText->SetText("");

	//�J�[�\��
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_TradeWindow->transform);
	_Cursor->transform->SetLocalPosition(Vector3(-10, 10, 0));

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
		//�A�C�e���I��
		if (VPadInput->IsPush(fbEngine::VPad::ButtonUp))
		{
			_SetIndex((_Select > 0) ? _Select - 1 : _DisplayItemNum - 1);
		}
		else if (VPadInput->IsPush(fbEngine::VPad::ButtonDown))
		{
			_SetIndex((_Select + 1) % _DisplayItemNum);
		}
		//���ʌ���
		_UpdateTradeNum();
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

	_TradeNum = 1;
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
	if (_SelectItem->TypeID == Item::ItemCodeE::Item)
	{
		if (VPadInput->IsPush(fbEngine::VPad::ButtonRight))
		{
			int maxNum = (_SaveState == Shop::ShopStateE::Buy) ? 99 : ((ConsumptionItem*)_DisplayList.at(_Select))->GetHoldNum();
			_TradeNum = min(maxNum, _TradeNum + 1);
			_UpdateText();
		}
		else if (VPadInput->IsPush(fbEngine::VPad::ButtonLeft))
		{
			_TradeNum = max(1, _TradeNum - 1);
			_UpdateText();
		}
	}
}

void ShopS_Trade::_CreateMenu()
{
	//�e�L�X�g�����B
	while (_MenuTexts.size() <= 30)
	{
		//�C���X�^���X���B
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _TradeWindow->GetPriorty());

		text->SetFontSize(50);
		text->SetAnchor(fbText::TextAnchorE::MiddleLeft);
		text->transform->SetParent(_TradeWindow->transform);

		//���X�g�ɒǉ��B
		_MenuTexts.push_back(text);

		TextObject* money = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _TradeWindow->GetPriorty());

		money->SetFontSize(50);
		money->SetAnchor(fbText::TextAnchorE::MiddleRight);
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
		_DisplayList = _Shop->_ItemList;
	else if (_SaveState == Shop::ShopStateE::Sell)
		_DisplayList = INSTANCE(Inventory)->GetInventoryList(static_cast<Item::ItemCodeE>(_DisplayType));
	
	_DisplayItemNum = 0;
	for (int i = 0; i < _DisplayList.size(); i++)
	{
		if (_DisplayList.at(i) != nullptr)
			_DisplayItemNum++;
	}		

	//�e�L�X�g�̕����X�V�B
	_UpdateText();
	//�I�����Ă���Y�����X�V�B
	_SetIndex(_Select);
	//�\������A�C�e�����X�N���[���B
	_ScrollDisplayItem();
}

void ShopS_Trade::_SetIndex(int idx)
{
	if (_DisplayItemNum > 0)
	{
		//�I�����Ă���Y�����ݒ�B
		_Select = min(idx, _DisplayItemNum - 1);
		//�A�C�e����ݒ�B
		_SelectItem = _DisplayList.at(_Select)->GetInfo();
		//�A�C�e���̏��𑗂�B
		_SendItemInfo(_SelectItem);

		//���X�g�̕\���X�V�B
		if (_Select >= _MinIdx + DISPLAY_ITEM_NUM)
			_SetMinIndex(_Select - (DISPLAY_ITEM_NUM - 1));
		else if (_Select < _MinIdx)
			_SetMinIndex(_Select);

		//�J�[�\�������炷�B
		float posx = -(_TradeWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;
		int displayidx = _Select - _MinIdx + 1;
		float posy = _MenuListHeight * displayidx + _MenuListHeight*0.5f;
		_Cursor->transform->SetLocalPosition(posx, posy, 0);
	}
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
		auto &item = _DisplayList[i];
		//�e�L�X�g�ݒ�B
		char menu[256];
		sprintf(menu, "%s", item->GetInfo()->Name);
		_MenuTexts[i]->SetText(menu);

		//�����ݒ�B
		height += _MenuTexts[i]->GetLength().y;
		//�ő�̍�����ێ��B
		_MenuListHeight = max(_MenuListHeight, _MenuTexts[i]->GetLength().y);

		char info[256];
		if (item->GetInfo()->TypeID == Item::ItemCodeE::Item)
		{
			sprintf(info, "%2d %2d %6d$", ((ConsumptionItem*)item)->GetHoldNum(), _TradeNum, item->GetInfo()->Value*_TradeNum);
		}
		else
		{
			sprintf(info, "%2d %6d$", _TradeNum, item->GetInfo()->Value*_TradeNum);
		}
		_MoneyTexts[i]->SetText(info);
	}
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
		_MenuTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * count, 0);
		_MoneyTexts[i]->transform->SetLocalPosition(_TradeWindow->GetSize().x - 60, 0, 0);
	}
}

void ShopS_Trade::_SendItemInfo(Item::BaseInfo* info)
{
	try
	{
		//�p�����[�^�̕�������i�[�B
		char text[256];
		ZeroMemory(text, 256);

		if (info->TypeID == Item::ItemCodeE::Weapon)
		{
			//����ɃL���X�g�B
			auto weapon = (Item::WeaponInfo*)info;
			//���݂̑����擾�B
			auto equip = weapon;

			sprintf(text, "ATK %4d -> %s%4d</color>\nMAG %4d -> %s%4d</color>\nDEX %4d -> %s%4d</color>\nCRT %4d -> %s%4d</color>",
				equip->Atk, _CalcColorCode(1), weapon->Atk,
				equip->MagicAtk, _CalcColorCode(-1), weapon->MagicAtk,
				equip->Dex, _CalcColorCode(1), weapon->Dex,
				equip->CriticalDamage, _CalcColorCode(0), weapon->CriticalDamage);
		}

		//�������𑗐M�B
		_Shop->SetDescriptionText(info->Description);
		//�p�����[�^��\���B
		_ParmText->SetText(text);
	}
	catch (const std::exception&)
	{

	}
}

char * ShopS_Trade::_CalcColorCode(int diff)
{
	if (diff > 0)
		return  "<color=0fffffff>";
	else if (diff < 0)
		return  "<color=ff0000ff>";
	return "<color=ffffffff>";
}

void ShopS_Trade::_Decision()
{
	if (_DisplayItemNum > 0)
	{
		//�e�L�X�g�B
		char msg[256];
		sprintf(msg, "%s �� %d �ł��ˁB\n�S���� %d$ �ɂȂ�܂��B", _SelectItem->Name, _TradeNum, _SelectItem->Value*_TradeNum);
		//�֐���ݒ�B
		if (_SaveState == Shop::ShopStateE::Buy)
		{
			//����������Ă��邩�H
			if (INSTANCE(Inventory)->GetPlayerMoney() >= _SelectItem->Value*_TradeNum)
			{
				_Shop->_ShopFunc = std::bind(&ShopS_Trade::BuyItem, this);
				//�w���m�F��ʂ��o���B
				_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
			}
			else
			{
				//�w���ł��Ȃ��|��\���B
				sprintf(msg, "����������܂����B");
			}
		}
		else if (_SaveState == Shop::ShopStateE::Sell)
		{
			_Shop->_ShopFunc = std::bind(&ShopS_Trade::SellItem, this);
			//�̔��m�F��ʂ��o���B
			_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
		}
		_Shop->SetDescriptionText(msg);
	}
}

void ShopS_Trade::BuyItem()
{
	//�A�C�e���̒l�i�������𕥂��B
	_Shop->Pay(_SelectItem->Value * _TradeNum);
	//�C���x���g���֒ǉ��B
	if (_SelectItem->TypeID == Item::ItemCodeE::Item)
	{
		//�A�C�e����ǉ��B
		INSTANCE(Inventory)->AddItem((Item::ItemInfo*)_SelectItem, _TradeNum);
	}
	else
	{
		//�����i��ǉ��B
		INSTANCE(Inventory)->AddEquipment(_SelectItem, false);
	}
	_Shop->SetDescriptionText("�܂��ǂ���B");
}

void ShopS_Trade::SellItem()
{
	//�C���x���g������r���B
	if (INSTANCE(Inventory)->SubHoldNum(_SelectItem, _TradeNum))
	{
		//�폜����Ă����Ȃ烊�X�g�X�V�B
		_UpdateList();
	}
	//�A�C�e���̒l�i��������Ⴄ�B
	_Shop->Pay(-_SelectItem->Value * _TradeNum);
	_Shop->SetDescriptionText("�܂��ǂ���B");
}
