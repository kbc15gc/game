#include "stdafx.h"
#include "ShopS_Confirmation.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

ShopS_Confirmation::ShopS_Confirmation(Shop * shop) : IShopState(shop) 
{
	//背景
	_ConfirmationWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectWindow", 8);
	_ConfirmationWindow->SetTexture(LOADTEXTURE("window.png"));
	_ConfirmationWindow->SetSize(Vector2(256, 128));
	_ConfirmationWindow->transform->SetPosition(Vector3(1050, 600, 0));

	//カーソル
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_ConfirmationWindow->transform);
	_Cursor->SetSize(Vector2(32, 32));
	_Cursor->transform->SetLocalPosition(Vector3(0, 0, 0));

	//テキスト。
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", 8);

	_Text->SetText("はい\nいいえ");
	_Text->SetFontSize(50);
	_Text->transform->SetParent(_ConfirmationWindow->transform);
	_Text->SetAnchor(fbText::TextAnchorE::UpperLeft);
	_Text->transform->SetLocalPosition(Vector3(-_Text->GetLength().x/2, -40, 0));

	//ウィンドウを非アクティブに
	_ConfirmationWindow->SetActive(false, true);
}

void ShopS_Confirmation::Update()
{
	//カーソル移動。
	const int max = 2;
	if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonUp, 0.2f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKU, 0.2f))
	{
		_Select = (_Select > 0) ? _Select - 1 : max - 1;
	}
	else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonDown , 0.2f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKD, 0.2f))
	{
		_Select = (_Select + 1) % max;
	}
	//カーソルをずらす。
	_Cursor->transform->SetLocalPosition(Vector3(-_Text->GetLength().x / 2 - _Cursor->GetSize().x, 60 * _Select - 30, 0));

	//決定(仮)
	if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
	{
		if(_Select == 0)
		{
			//設定された関数実行。
			_Shop->_ShopFunc();
			//もどる。
			_Shop->_ChangeState(_Caller);
		}
		else if(_Select == 1)
		{
			_Shop->_ChangeState(_Caller);
		}
	}

	//キャンセル。
	if (VPadInput->IsPush(fbEngine::VPad::ButtonB))
	{
		_Shop->_ChangeState(_Caller);
	}
}

void ShopS_Confirmation::EnterState()
{
	_Select = 0;
	//ウィンドウを非アクティブに
	_ConfirmationWindow->SetActive(true, true);
}

void ShopS_Confirmation::ExitState()
{
	//ウィンドウを非アクティブに
	_ConfirmationWindow->SetActive(false, true);
}

void ShopS_Confirmation::DiveState()
{
}
