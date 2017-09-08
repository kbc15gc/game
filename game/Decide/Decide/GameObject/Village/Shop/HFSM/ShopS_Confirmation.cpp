#include "stdafx.h"
#include "ShopS_Confirmation.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

ShopS_Confirmation::ShopS_Confirmation(Shop * shop) : IShopState(shop) 
{
	//�w�i
	_ConfirmationWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectWindow", 8);
	_ConfirmationWindow->SetTexture(LOADTEXTURE("ShopSelect.png"));
	_ConfirmationWindow->transform->SetPosition(Vector3(1050, 400, 0));

	//�J�[�\��
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_ConfirmationWindow->transform);
	_Cursor->transform->SetLocalPosition(Vector3(0, 0, 0));

	//�E�B���h�E���A�N�e�B�u��
	_ConfirmationWindow->SetActive(false, true);
}

void ShopS_Confirmation::Update()
{
	//�J�[�\���ړ��B
	static int idx = 0;
	const int max = 2;
	if ((KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU)))
	{
		idx = (idx > 0) ? idx - 1 : max - 1;
	}
	else if ((KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD)))
	{
		idx = (idx + 1) % max;
	}
	//�J�[�\�������炷�B
	_Cursor->transform->SetLocalPosition(Vector3(-60, 80 * idx - 40, 0));

	//����(��)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		if(idx == 0)
		{
			//�ݒ肳�ꂽ�֐����s�B
			_Shop->_ShopFunc(_Shop->_SelectItem);
			//���ǂ�B
			_Shop->_ChangeState(_BeforeState);
		}
		else if(idx == 1)
		{
			_Shop->_ChangeState(_BeforeState);
		}
	}

	//�L�����Z���B
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_Shop->_ChangeState(_BeforeState);
	}
}

void ShopS_Confirmation::EnterState()
{
	//�E�B���h�E���A�N�e�B�u��
	_ConfirmationWindow->SetActive(true, true);
	_BeforeState = _Shop->_State;
}

void ShopS_Confirmation::ExitState()
{
	//�E�B���h�E���A�N�e�B�u��
	_ConfirmationWindow->SetActive(false, true);
	_BeforeState = Shop::ShopStateE::None;
}

void ShopS_Confirmation::DiveState()
{
}
