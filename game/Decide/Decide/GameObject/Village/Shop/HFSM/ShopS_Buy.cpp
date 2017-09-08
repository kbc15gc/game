#include "stdafx.h"
#include "ShopS_Buy.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

ShopS_Buy::ShopS_Buy(Shop * shop) :IShopState(shop)
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

void ShopS_Buy::Update()
{
	//�J�[�\���ړ��B
	MoveMenuCursor();

	//����(��)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//�w���m�F��ʂ��o���B
		_Shop->_ChangeState(Shop::ShopStateE::Confirmation);
	}
	//�L�����Z���B
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Select);
	}
}

void ShopS_Buy::EnterState()
{
	idx = 0;
	//���j���[���쐬�B
	_CreateMenu();
	UpdateDisplayItem();
	//�E�B���h�E���A�N�e�B�u�ɂ���B
	_BuyWindow->SetActive(true, true);
}

void ShopS_Buy::ExitState()
{
	//���j���[�����B
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
		//�V���b�v�̕i���낦�Ɠ����ʁA�e�L�X�g�𐶐��B
		auto items = _Shop->_ItemList;
		FOR(i, items.size())
		{
			//�C���X�^���X���B
			TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _BuyWindow->GetPriorty());

			//�e�L�X�g�ݒ�B
			char t[256];
			sprintf(t, "���O:%s,  �l�i:%d$", items[i]->Name, items[i]->Value);
			text->SetString(t);
			text->SetFontSize(50);
			text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);

			//�����ݒ�B
			height += text->GetLength().y;
			//�ő�̍�����ێ��B
			_MenuListHeight = max(_MenuListHeight, text->GetLength().y);
			//�E�B���h�E�̎q�ɓo�^�B
			text->transform->SetParent(_BuyWindow->transform);

			//���X�g�ɒǉ��B
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
	//���ڂ��ύX���ꂽ���H
	bool change = false;

	if (change = (KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU)))
	{
		idx = (idx > 0) ? idx - 1 : _Shop->_ItemList.size() - 1;
	}
	else if (change = (KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD)))
	{
		idx = (idx + 1) % _Shop->_ItemList.size();
	}

	//�ύX���ꂽ���H
	if (change)
	{
		//�J�[�\�������炷�B
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x / 2;

		float posy = _MenuListHeight * (idx + 1) + _MenuListHeight*0.5f;
		_Cursor->transform->SetLocalPosition(posx, posy, 0);

		//�A�C�e���̏��𑗂�B
		_Shop->SetDescriptionText(_Shop->_ItemList[idx]->Description);
	}
}

void ShopS_Buy::UpdateDisplayItem()
{
	//��荇������x�S�Ĕ�\���ɂ���B
	for each (auto text in _MenuTexts)
	{
		text->SetActive(false);
	}

	//�\���̍ŏ��Y��������J�E���g���\������B
	for (int i = _MinIdx; i < _MinIdx + (DISPLAY_ITEM_NUM-1) && i < _MenuTexts.size(); i++)
	{
		_MenuTexts[i]->SetActive(true);
		float posx = -(_BuyWindow->GetSize().x / 2) + _Cursor->GetSize().x;
		_MenuTexts[i]->transform->SetLocalPosition(posx, _MenuListHeight * i, 0);
	}
}
