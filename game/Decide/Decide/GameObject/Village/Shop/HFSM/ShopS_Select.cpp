#include "stdafx.h"
#include "ShopS_Select.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

ShopS_Select::ShopS_Select(Shop * shop) :IShopState(shop)
{
	//背景
	_SelectWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectWindow", 8);
	_SelectWindow->SetTexture(LOADTEXTURE("window.png"));
	_SelectWindow->SetSize(Vector2(256, 128));
	_SelectWindow->transform->SetPosition(Vector3(1050, 100, 0));

	//カーソル
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->transform->SetParent(_SelectWindow->transform);

	//テキスト。
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", 8);

	_Text->SetText("かう\nうる");
	_Text->SetFontSize(50);
	_Text->SetAnchor(fbText::TextAnchorE::MiddleLeft);
	_Text->transform->SetParent(_SelectWindow->transform);
	_Text->transform->SetLocalPosition(Vector3(-_Text->GetLength().x/2, -40, 0));


	//ウィンドウを非アクティブに
	_SelectWindow->SetActive(false, true);
}

void ShopS_Select::Update()
{
	//現在選択している項目
	const int MAX_SELECT = 2;
	if (KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU))
	{
		select = select > 0 ? select - 1 : MAX_SELECT - 1;
	}
	else if (KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
	{
		select = (select + 1) % MAX_SELECT;
	}
	//カーソルをずらす。
	_Cursor->transform->SetLocalPosition(Vector3(-_Text->GetLength().x/2 - _Cursor->GetSize().x, 60 * select - 30, 0));

	//決定(仮)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		switch (select)
		{
		case 0:
			_Shop->_ChangeState(Shop::ShopStateE::Buy);
			break;
		case 1:
			_Shop->_ChangeState(Shop::ShopStateE::Sell);
			break;
		case 2:
			break;
		default:
			break;
		}
	}
	//キャンセル。
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Close);
	}
}

void ShopS_Select::EnterState()
{
	//ウィンドウをアクティブにする。
	_SelectWindow->SetActive(true, true);
	//テキスト設定。
	_Shop->SetDescriptionText("いらっしゃい、なにか買っていくかい？\nそれとも買い取ろうか？");
}

void ShopS_Select::ExitState()
{
	_SelectWindow->SetActive(false, true);
}

void ShopS_Select::DiveState()
{
}
