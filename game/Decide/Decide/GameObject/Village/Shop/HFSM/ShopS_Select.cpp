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
	_SelectWindow->transform->SetPosition(Vector3(1050, 170, 0));

	//カーソル
	_Cursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 8);
	_Cursor->SetTexture(LOADTEXTURE("ShopCursor.png"));
	_Cursor->SetSize(Vector2(32, 32));
	_Cursor->transform->SetParent(_SelectWindow->transform);

	//テキスト。
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", 8);

	_Text->SetText("かう\nうる");
	_Text->SetFontSize(50);
	_Text->SetAnchor(fbText::TextAnchorE::UpperLeft);
	_Text->transform->SetParent(_SelectWindow->transform);
	_Text->transform->SetLocalPosition(Vector3(-_Text->GetLength().x/2, -40, 0));


	//ウィンドウを非アクティブに
	_SelectWindow->SetActive(false, true);
}

void ShopS_Select::Update()
{
	//現在選択している項目
	const int MAX_SELECT = 2;
	if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonUp, 0.2f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKU, 0.2f))
	{
		select = select > 0 ? select - 1 : MAX_SELECT - 1;
	}
	else if (VPadInput->KeyRepeat(fbEngine::VPad::ButtonDown, 0.2f) || XboxInput(0)->AnalogRepeat(AnalogE::L_STICKD, 0.2f))
	{
		select = (select + 1) % MAX_SELECT;
	}
	//カーソルをずらす。
	_Cursor->transform->SetLocalPosition(Vector3(-_Text->GetLength().x/2 - _Cursor->GetSize().x, 60 * select - 30, 0));

	//決定(仮)
	if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
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
	if (VPadInput->IsPush(fbEngine::VPad::ButtonB))
	{
		_Shop->_ChangeState(Shop::ShopStateE::Close);
	}
}

void ShopS_Select::EnterState()
{
	//ウィンドウをアクティブにする。
	_SelectWindow->SetActive(true, true);
	//テキスト設定。ボイス再生。
	_Shop->SetDescription("いらっしゃい、なにか買っていくかい？\nそれとも買い取ろうか？","Asset/Sound/NPC/SHOP/Mic1_49.wav");
}

void ShopS_Select::ExitState()
{
	select = 0;
	_SelectWindow->SetActive(false, true);
}

void ShopS_Select::DiveState()
{
	_SelectWindow->SetActive(false, true);
}
