#include "stdafx.h"
#include "ShopS_Trade.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

ShopS_Trade::ShopS_Trade(Shop * shop) :IShopState(shop)
{
	//�w�i	
	_BuyWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyWindouw", 8);
	_BuyWindow->SetTexture(LOADTEXTURE("window.png"));
	_BuyWindow->SetSize(Vector2(800, 400));
	_BuyWindow->transform->SetPosition(Vector3(450, 50, 0));
	_BuyWindow->SetPivot(Vector2(0.5f, 0.0f));

	//�J�[�\��
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuyCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_BuyWindow->transform);
	_Cursor->transform->SetLocalPosition(Vector3(-10, 10, 0));

	//�E�B���h�E���A�N�e�B�u��
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
	if ((KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU)))
	{
		SetIndex((_Select > 0) ? _Select - 1 : _ItemList.size() - 1);
	}
	else if ((KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD)))
	{
		SetIndex((_Select + 1) % _ItemList.size());
	}

	//����(��)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		Decision();
	}
	//�L�����Z���B
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Select);
	}
}

void ShopS_Trade::EnterState()
{
	//�E�B���h�E���A�N�e�B�u�ɂ���B
	_BuyWindow->SetActive(true, true);
	//���j���[���쐬�B
	_CreateMenu();
	//�e�L�X�g�ݒ�B
	UpdateText();
	//
	SetIndex(_Select);
	//�\���X�V�B
	ScrollDisplayItem();

	_SelectNum = 1;
}

void ShopS_Trade::ExitState()
{
	//
	SetIndex(0);
	SetMinIndex(0);
	//���j���[�����B
	_CloseMenu();
	_BuyWindow->SetActive(false, true);
}

void ShopS_Trade::DiveState()
{
}

void ShopS_Trade::_CreateMenu()
{
	//�e�L�X�g�����B
	while (_MenuTexts.size() <= 30)
	{
		//�C���X�^���X���B
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

		text->SetFontSize(50);
		text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);
		text->transform->SetParent(_BuyWindow->transform);

		//���X�g�ɒǉ��B
		_MenuTexts.push_back(text);

		TextObject* money = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

		money->SetFontSize(50);
		money->SetFormat((unsigned int)fbText::TextFormatE::RIGHT);
		money->transform->SetParent(text->transform);

		_MoneyTexts.push_back(money);
	}
	//�V���b�v�̕i���낦�Ɠ����ʁA�e�L�X�g�𐶐��B
	if (_Shop->_State == Shop::ShopStateE::Buy)
	{
		_ItemList = _Shop->_ItemList;
	}
	else if (_Shop->_State == Shop::ShopStateE::Sell)
	{
		//_ItemList = INSTANCE(Inventory)->GetInfoList();
	}
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
	if (_ItemList.size() > 0)
	{
		//�I�����Ă���Y�����ݒ�B
		_Select = idx;

		//���X�g�̕\���X�V�B
		if (_Select >= _MinIdx + DISPLAY_ITEM_NUM)
			SetMinIndex(_Select - (DISPLAY_ITEM_NUM - 1));
		else if (_Select < _MinIdx)
			SetMinIndex(_Select);

		//�J�[�\�������炷�B
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;
		int displayidx = _Select - _MinIdx + 1;
		float posy = _MenuListHeight * displayidx + _MenuListHeight*0.5f;
		_Cursor->transform->SetLocalPosition(posx, posy, 0);

		//�A�C�e���̏��𑗂�B

		_Shop->SetDescriptionText(_ItemList.at(_Select)->GetInfo()->Description);
	}
}

void ShopS_Trade::SetMinIndex(int minidx)
{
	_MinIdx = max(0, minidx);
	ScrollDisplayItem();
}

void ShopS_Trade::ScrollDisplayItem()
{
	//��荇������x�S�Ĕ�\���ɂ���B
	_CloseMenu();

	//�\���̍ŏ��Y��������J�E���g���\������B
	for (int i = _MinIdx,count = 1; i < _MinIdx + DISPLAY_ITEM_NUM && i < _ItemList.size(); i++,count++)
	{
		_MenuTexts[i]->SetActive(true,true);
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x;
		_MenuTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * count, 0);
		_MoneyTexts[i]->transform->SetLocalPosition(_BuyWindow->GetSize().x - 60, 0, 0);
	}
}

void ShopS_Trade::UpdateText()
{
	//������B
	_CloseMenu();

	float height = 0.0f;
	_MenuListHeight = 0.0f;
	//�e�L�X�g�ݒ�B
	FOR(i, _ItemList.size())
	{
		_MenuTexts[i]->SetActive(true);
		//�e�L�X�g�ݒ�B
		char value[256];
		sprintf(value, "%d$", _ItemList[i]->GetInfo()->Value);
		_MenuTexts[i]->SetString(_ItemList[i]->GetInfo()->Name);
		_MoneyTexts[i]->SetString(value);
		//�����ݒ�B
		height += _MenuTexts[i]->GetLength().y;
		//�ő�̍�����ێ��B
		_MenuListHeight = max(_MenuListHeight, _MenuTexts[i]->GetLength().y);
	}
}

void ShopS_Trade::Decision()
{
	//�A�C�e����ݒ�B
	_Shop->_SelectItem = _ItemList[_Select]->GetInfo();
	//�֐���ݒ�B
	if (_Shop->_State == Shop::ShopStateE::Buy)
	{
		//����������Ă��邩�H
		if (INSTANCE(Inventory)->GetPlayerMoney() >= _ItemList[_Select]->GetInfo()->Value)
		{
			_Shop->_ShopFunc = std::bind(&ShopS_Trade::BuyItem, this, std::placeholders::_1);
			//�e�L�X�g�ݒ�B
			char t[256];
			sprintf(t, "%s �� %d �ł��ˁB\n�S���� %d$ �ɂȂ�܂��B", _Shop->_SelectItem->Name, _SelectNum, _Shop->_SelectItem->Value);
			_Shop->SetDescriptionText(t);
		}
		else
		{
			//�w���ł��Ȃ��|��\���B
			_Shop->SetDescriptionText("����������܂����B");
		}
	}
	else if (_Shop->_State == Shop::ShopStateE::Sell)
	{
		_Shop->_ShopFunc = std::bind(&ShopS_Trade::SellItem, this, std::placeholders::_1);//�e�L�X�g�ݒ�B
		char t[256];
		sprintf(t, "%s �� %d �ł��ˁB\n�S���� %d$ �ɂȂ�܂��B", _Shop->_SelectItem->Name, _SelectNum, _Shop->_SelectItem->Value);
		_Shop->SetDescriptionText(t);
	}
	//�w���m�F��ʂ��o���B
	_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
}

void ShopS_Trade::BuyItem(Item::BaseInfo *info)
{
	//�A�C�e���̒l�i�������𕥂��B
	INSTANCE(Inventory)->SubtractPlayerMoney(info->Value);
	//�C���x���g���֒ǉ��B
	INSTANCE(Inventory)->AddItem((Item::ItemCodeE)info->TypeID,info);
}

void ShopS_Trade::SellItem(Item::BaseInfo *info)
{
	//�C���x���g������r���B
	INSTANCE(Inventory)->SubHoldNum(info,_SelectNum);
	//�e�L�X�g�̍X�V�B
	UpdateText();
	//�A�C�e���̒l�i��������Ⴄ�B
	INSTANCE(Inventory)->SubtractPlayerMoney(-info->Value);
}
