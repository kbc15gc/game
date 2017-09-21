#include "stdafx.h"
#include "ShopS_Confirmation.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

ShopS_Confirmation::ShopS_Confirmation(Shop * shop) : IShopState(shop) 
{
	//�w�i
	_ConfirmationWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectWindow", 8);
	_ConfirmationWindow->SetTexture(LOADTEXTURE("window.png"));
	_ConfirmationWindow->SetSize(Vector2(256, 128));
	_ConfirmationWindow->transform->SetPosition(Vector3(1050, 550, 0));

	//�J�[�\��
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_ConfirmationWindow->transform);
	_Cursor->transform->SetLocalPosition(Vector3(0, 0, 0));

	//�e�L�X�g�B
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", 8);

	_Text->SetText("�͂�\n������");
	_Text->SetFontSize(50);
	_Text->transform->SetParent(_ConfirmationWindow->transform);
	_Text->SetAnchor(fbText::TextAnchorE::MiddleLeft);
	_Text->transform->SetLocalPosition(Vector3(-_Text->GetLength().x/2, -40, 0));

	//�E�B���h�E���A�N�e�B�u��
	_ConfirmationWindow->SetActive(false, true);
}

void ShopS_Confirmation::Update()
{
	//�J�[�\���ړ��B
	const int max = 2;
	if ((KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU)))
	{
		_Select = (_Select > 0) ? _Select - 1 : max - 1;
	}
	else if ((KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD)))
	{
		_Select = (_Select + 1) % max;
	}
	//�J�[�\�������炷�B
	_Cursor->transform->SetLocalPosition(Vector3(-_Text->GetLength().x / 2 - _Cursor->GetSize().x, 60 * _Select - 30, 0));

	//����(��)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		if(_Select == 0)
		{
			//�ݒ肳�ꂽ�֐����s�B
			_Shop->_ShopFunc();
			//���ǂ�B
			_Shop->_ChangeState(_Caller);
		}
		else if(_Select == 1)
		{
			_Shop->_ChangeState(_Caller);
		}
	}

	//�L�����Z���B
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_Shop->_ChangeState(_Caller);
	}
}

void ShopS_Confirmation::EnterState()
{
	_Select = 0;
	//�E�B���h�E���A�N�e�B�u��
	_ConfirmationWindow->SetActive(true, true);
}

void ShopS_Confirmation::ExitState()
{
	//�E�B���h�E���A�N�e�B�u��
	_ConfirmationWindow->SetActive(false, true);
}

void ShopS_Confirmation::DiveState()
{
}
