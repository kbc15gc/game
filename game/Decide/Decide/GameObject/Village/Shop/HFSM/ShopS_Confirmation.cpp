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
	_ConfirmationWindow->transform->SetPosition(Vector3(1050, 600, 0));

	//�J�[�\��
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_ConfirmationWindow->transform);
	_Cursor->SetSize(Vector2(32, 32));
	_Cursor->transform->SetLocalPosition(Vector3(0, 0, 0));

	//�e�L�X�g�B
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", 8);

	_Text->SetText("�͂�\n������");
	_Text->SetFontSize(50);
	_Text->transform->SetParent(_ConfirmationWindow->transform);
	_Text->SetAnchor(fbText::TextAnchorE::UpperLeft);
	_Text->transform->SetLocalPosition(Vector3(-_Text->GetLength().x/2, -40, 0));

	//�E�B���h�E���A�N�e�B�u��
	_ConfirmationWindow->SetActive(false, true);
}

void ShopS_Confirmation::Update()
{
	//�J�[�\���ړ��B
	const int max = 2;
	if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonUp, 0.2f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKU, 0.2f))
	{
		_Select = (_Select > 0) ? _Select - 1 : max - 1;
	}
	else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonDown , 0.2f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKD, 0.2f))
	{
		_Select = (_Select + 1) % max;
	}
	//�J�[�\�������炷�B
	_Cursor->transform->SetLocalPosition(Vector3(-_Text->GetLength().x / 2 - _Cursor->GetSize().x, 60 * _Select - 30, 0));

	//����(��)
	if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
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
	if (VPadInput->IsPush(fbEngine::VPad::ButtonB))
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
