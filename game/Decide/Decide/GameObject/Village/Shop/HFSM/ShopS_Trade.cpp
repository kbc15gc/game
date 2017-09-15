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

		//����(��)
		if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
		{
			Decision();
		}
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
	_SaveState = _Shop->_State;
	//���j���[���쐬�B
	_CreateMenu();
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
		//���X�g�̕\���X�V�B
		if (idx >= _MinIdx + DISPLAY_ITEM_NUM)
			SetMinIndex(idx - (DISPLAY_ITEM_NUM - 1));
		else if (idx < _MinIdx)
			SetMinIndex(idx);

		//�J�[�\�������炷�B
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;
		int displayidx = idx - _MinIdx + 1;
		float posy = _MenuListHeight * displayidx + _MenuListHeight*0.5f;
		_Cursor->transform->SetLocalPosition(posx, posy, 0);

		//�A�C�e���̏��𑗂�B
		if (_Select != idx)
			_Shop->SetDescriptionText(_DisplayList.at(idx)->GetInfo()->Description);

		//�I�����Ă���Y�����ݒ�B
		_Select = idx;

		//�I�𐔏������B
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
	//��荇������x�S�Ĕ�\���ɂ���B
	_CloseMenu();

	//�\���̍ŏ��Y��������J�E���g���\������B
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
	//�\�����郊�X�g�擾�B
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
				//null�`�F�b�N�B
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
	//������B
	_CloseMenu();

	float height = 0.0f;
	_MenuListHeight = 0.0f;
	//�e�L�X�g�ݒ�B
	FOR(i, _DisplayList.size())
	{
		_MenuTexts[i]->SetActive(true);
		//�e�L�X�g�ݒ�B
		char menu[256];
		sprintf(menu, "%s", _DisplayList[i]->GetInfo()->Name);
		_MenuTexts[i]->SetString(menu);

		char info[256];
		if (_SaveState == Shop::ShopStateE::Sell)
			sprintf(info, "%6d$%4d��", _DisplayList[i]->GetInfo()->Value, _DisplayList[i]->GetHoldNum());
		else
			sprintf(info, "%6d$", _DisplayList[i]->GetInfo()->Value);
		_MoneyTexts[i]->SetString(info);
		//�����ݒ�B
		height += _MenuTexts[i]->GetLength().y;
		//�ő�̍�����ێ��B
		_MenuListHeight = max(_MenuListHeight, _MenuTexts[i]->GetLength().y);
	}
}

void ShopS_Trade::Decision()
{
	if (_DisplayList.size() > 0)
	{
		//�A�C�e����ݒ�B
		_SelectItem = _DisplayList[_Select]->GetInfo();
		//�e�L�X�g�B
		char msg[256];
		sprintf(msg, "%s �� %d �ł��ˁB\n�S���� %d$ �ɂȂ�܂��B", _SelectItem->Name, _SelectNum, _SelectItem->Value*_SelectNum);
		//�֐���ݒ�B
		if (_SaveState == Shop::ShopStateE::Buy)
		{
			//����������Ă��邩�H
			if (INSTANCE(Inventory)->GetPlayerMoney() >= _DisplayList[_Select]->GetInfo()->Value)
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
	INSTANCE(Inventory)->SubtractPlayerMoney(_SelectItem->Value * _SelectNum);
	//�C���x���g���֒ǉ��B
	INSTANCE(Inventory)->AddItem((Item::ItemCodeE)_SelectItem->TypeID, _SelectItem, _SelectNum);
	_Shop->SetDescriptionText("�܂��ǂ���B");
}

void ShopS_Trade::SellItem()
{
	//�C���x���g������r���B
	if (INSTANCE(Inventory)->SubHoldNum(_SelectItem, _SelectNum))
	{
		//�폜����Ă����Ȃ烊�X�g�X�V�B
		UpdateList();
	}
	//�A�C�e���̒l�i��������Ⴄ�B
	INSTANCE(Inventory)->SubtractPlayerMoney(-_SelectItem->Value * _SelectNum);
}
