/**
* 歴史変更メニュー画面クラスの実装.
*/
#include"stdafx.h"
#include "HistoryMenu.h"

#include "fbEngine\_Object\_GameObject\TextObject.h"

#include"..\HistoryBook\HistoryBook.h"

#include "GameObject\History\HistoryManager.h"


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

		for (auto& it : _Chip2DList)
		{
			it->SetActive(false);
		}
	}
}

/**
* チップを追加.
*/
void HistoryMenu::AddChip(ChipID chipID)
{
	Chip2D* chip2D = INSTANCE(GameObjectManager)->AddNew<Chip2D>("Chip2D", 9);
	chip2D->Start(chipID);
	_Chip2DList.push_back(chip2D);
}

/**
* 表示中の更新.
*/
void HistoryMenu::EnableUpdate()
{
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		//左トリガー.
		//_NowSelectLocation = min(_ReleaseLocation, _NowSelectLocation + 1);
	}
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		//右トリガー.
		//_NowSelectLocation = max(0, _NowSelectLocation - 1);
	}

	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		_NowSelectChip = min(max(0,_Chip2DList.size() - 1), _NowSelectChip + 1);
	}
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		_NowSelectChip = max(0, _NowSelectChip - 1);
	}

	//Aボタン押.
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A) || KeyBoardInput->isPush(DIK_J))
	{
		//存在していれば.
		if (_Chip2DList.size() != 0 && _Chip2DList[_NowSelectChip] != nullptr)
		{
			//現在指定している場所にチップを設定.
			INSTANCE(HistoryManager)->SetHistoryChip((LocationCodeE)_NowSelectLocation, _NowSlot, _Chip2DList[_NowSelectChip]->GetChipID());
			//搬入したチップを所持チップから削除.
			auto it = _Chip2DList.begin();
			it += _NowSelectChip;
			INSTANCE(GameObjectManager)->AddRemoveList(*it);
			_Chip2DList.erase(it);

			_NowSelectChip = min(max(0, _Chip2DList.size() - 1), _NowSelectChip);
			_NowSelectChip = max(0, _NowSelectChip);
		}
	}

	//BボタンもしくはKkeyが押されたら。
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B) || KeyBoardInput->isPush(DIK_K))
	{
		//歴史書からリストを取得。
		vector<HistoryPage*> pagelist = _HistoryBook->GetPageList();
		//取得したリストサイズが0以上なら削除処理を行う。
		if (pagelist.size() > 0)
		{
			HistoryPage* page = *pagelist.begin();
			page->ChangeState(HistoryPage::StateCodeE::PutOut);
			INSTANCE(HistoryManager)->PutOutPage(page);
			_HistoryBook->PutOutPage(page);
		}
	}

	//表示.
	_LocationNameRender->SetActive(true);

	//場所名描画.
	_LocationNameRender->SetString(LocationNameList[_NowSelectLocation].c_str());

	for (int i = 0; i < _Chip2DList.size(); i++)
	{
		_Chip2DList[i]->SetActive(true);
		if (i == _NowSelectChip)
		{
			_Chip2DList[i]->SetSize(Chip2D::SizeCodeE::Select);
		}
		else
		{
			_Chip2DList[i]->SetSize(Chip2D::SizeCodeE::NoSelect);
		}

		int len = i - _NowSelectChip;
		float offset = -150.0f;
		Vector3 pos = Vector3((g_WindowSize.x / 2.0f) + (offset * len), g_WindowSize.y - 10.0f, 0.0f);
		_Chip2DList[i]->transform->SetPosition(pos);
	}

}
