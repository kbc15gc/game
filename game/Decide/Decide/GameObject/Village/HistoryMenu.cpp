/**
* 歴史変更メニュー画面クラスの実装.
*/
#include"stdafx.h"
#include "HistoryMenu.h"

#include "fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\HistoryBook\HistoryBook.h"

/**
* 無名空間.
*/
namespace
{

	/** 場所名. */
	string LocationNameList[(int)HistoryMenu::LocationCodeE::LocationNum] =
	{
		"始まりの集落",
		"狩猟の村",
		"繁栄の町",
	};

}

/**
* 初期化.
*/
void HistoryMenu::Start()
{

	_LocationNameRender = INSTANCE(GameObjectManager)->AddNew<TextObject>("LocationNameRender", _Priority);

	_LocationNameRender->Initialize(L"", 80.0f, Color::white, fbSprite::SpriteEffectE::OUTLINE, STRING(fbText::TextStyleE::ＭＳ_明朝));
	

	//座標を設定.
	_LocationNameRender->transform->SetLocalPosition(Vector3(g_WindowSize.x / 2.0f, 50.0f, 0));
	//表示名を設定.
	_LocationNameRender->SetString(LocationNameList[_NowSelectLocation].c_str());

	//歴史書のポインタを取得.
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	_ReleaseLocation = (int)LocationCodeE::Prosperity;
}

/**
* 更新.
*/
void HistoryMenu::Update()
{
	if (_HistoryBook->GetNowState() == (int)HistoryBook::StateCodeE::Idol)
	{
		//表示.
		EnableUpdate();
	}
	else
	{
		//非表示.
		_LocationNameRender->SetActive(false);
	}
}

/**
* 表示中の更新.
*/
void HistoryMenu::EnableUpdate()
{
	//表示.
	_LocationNameRender->SetActive(true);

	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		//左トリガー.
		_NowSelectLocation = max(0, _NowSelectLocation - 1);
	}
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		//右トリガー.
		_NowSelectLocation = min(_ReleaseLocation, _NowSelectLocation + 1);
	}

	_LocationNameRender->SetString(LocationNameList[_NowSelectLocation].c_str());

}
