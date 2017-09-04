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

	//スプライトクラスを追加.
	_CursorSprite = AddComponent<Sprite>();
	_CursorSprite->SetTexture(LOADTEXTURE("cursor.png"));
}

/**
* 更新.
*/
void HistoryMenu::Update()
{
	if (_HistoryBook->GetNowState() == (int)HistoryBook::StateCodeE::Idol)
	{
		//表示.
		if (_IsOperation)
		{
			EnableUpdate();
		}
	}
	else
	{
		//非表示.
		_LocationNameRender->SetActive(false);
		_CursorSprite->enable = false;
		for (auto& it : _Chip2DList)
		{
			it->SetActive(false);
		}
	}

	if (!_IsOperation)
	{
		const float OperationTime = 2.0f;
		static float LocalTime = 0.0f;
		LocalTime += Time::DeltaTime();
		if (LocalTime >= OperationTime)
		{
			_IsOperation = true;
			_HistoryBook->SetIsOperation(_IsOperation);
			LocalTime = 0.0f;
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

	int len = _Chip2DList.size() - 1 - _NowSelectChip;
	float offset = -150.0f;
	Vector3 pos = Vector3((g_WindowSize.x / 2.0f) + (offset * len), g_WindowSize.y - 10.0f, 0.0f);
	chip2D->transform->SetPosition(pos);
	if (_Chip2DList.size() - 1 == _NowSelectChip)
	{
		chip2D->SetSize(Chip2D::SizeCodeE::Select);
	}
	else
	{
		chip2D->SetSize(Chip2D::SizeCodeE::NoSelect);
	}
}

/**
* 表示中の更新.
*/
void HistoryMenu::EnableUpdate()
{

	Vector2 cursorPos = Vector2((g_WindowSize.x / 2.0f), 0.0f);
	switch ((SelectCodeE)_SelectCode)
	{
		case SelectCodeE::Location:
			//場所選択中の更新.
			SelectLocationUpdate();
			cursorPos.y = 20.0f;
			break;
		case SelectCodeE::Page:
			//ページ選択中の更新.
			SelectPageUpdate();
			cursorPos.y = 150.0f;
			break;
		case SelectCodeE::Chip:
			//チップ選択中の更新.
			SelectChipUpdate();
			cursorPos.y = g_WindowSize.y - 250.0f;
			break;
	}

	const float ChangeTime = 0.3f;
	static float LocalTime = 0.0f;

	Vector2 LStick = XboxInput(0)->GetAnalog(AnalogInputE::L_STICK);
	LStick /= 32767.0f;
	if (LStick.y >= 0.3f)
	{
		LocalTime += Time::DeltaTime();

		if (LocalTime >= ChangeTime)
		{
			//上.
			_SelectCode = max((int)SelectCodeE::Location, _SelectCode - 1);
			LocalTime = 0.0f;
		}
	}
	else if (LStick.y <= -0.3f)
	{
		LocalTime += Time::DeltaTime();

		if (LocalTime >= ChangeTime)
		{
			//下.
			_SelectCode = min((int)SelectCodeE::Chip, _SelectCode + 1);
			LocalTime = 0.0f;
		}
	}
	else
	{
		LocalTime = ChangeTime;
	}

	_CursorSprite->transform->SetPosition(Vector3(cursorPos.x, cursorPos.y, 0.0f));

	//表示.
	_LocationNameRender->SetActive(true);
	_CursorSprite->enable = true;

	//場所名描画.
	_LocationNameRender->SetString(LocationNameList[_NowSelectLocation].c_str());

	for (int i = 0; i < _Chip2DList.size(); i++)
	{
		_Chip2DList[i]->SetActive(true);

		int len = i - _NowSelectChip;
		float offset = -150.0f;
		Vector3 pos = Vector3((g_WindowSize.x / 2.0f) + (offset * len), g_WindowSize.y - 10.0f, 0.0f);

		if (i == _NowSelectChip)
		{
			_Chip2DList[i]->SetMove(Chip2D::SizeCodeE::Select, pos);
		}
		else
		{
			_Chip2DList[i]->SetMove(Chip2D::SizeCodeE::NoSelect, pos);
		}
	}
}

/**
* 場所選択中の更新.
*/
void HistoryMenu::SelectLocationUpdate()
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
}

/**
* ページ選択中の更新.
*/
void HistoryMenu::SelectPageUpdate()
{
	int beforeLookPage = _NowLookPage;

	//ページを左右選択.
	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKR))
	{
		_NowLookPage = min(max(0, _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation).size() - 1), _NowLookPage + 1);
	}
	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKL))
	{
		_NowLookPage = max(0, _NowLookPage - 1);
	}
	
	//ローカルタイム.
	static Vector2 LOCAL_TIME = Vector2(0.0f, 0.0f);
	//キーリピートタイム.
	const float KEY_REPEAT_TIME = 0.8f;
	Vector2 InputAnalog = XboxInput(0)->GetAnalog(AnalogInputE::L_STICK);
	if (InputAnalog.x)
	{
		//ローカルタイムを計算.
		LOCAL_TIME.x += Time::DeltaTime();

		if (LOCAL_TIME.x >= KEY_REPEAT_TIME)
		{
			_NowLookPage = min(max(0, _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation).size() - 1), _NowLookPage + 1);
		}
	}
	else
	{
		LOCAL_TIME.x = 0.0f;
	}
	if (InputAnalog.y)
	{
		//ローカルタイムを計算.
		LOCAL_TIME.y += Time::DeltaTime();

		if (LOCAL_TIME.y >= KEY_REPEAT_TIME)
		{
			_NowLookPage = max(0, _NowLookPage - 1);
		}
	}
	else
	{
		LOCAL_TIME.y = 0.0f;
	}

	if (_HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation).size() > 0)
	{
		if (beforeLookPage < _NowLookPage)
		{
			_HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation)[beforeLookPage]->SetRotAngle(90.0f);
			_HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation)[beforeLookPage]->ChangeState(HistoryPage::StateCodeE::Turn);
		}
		else if (beforeLookPage > _NowLookPage)
		{

			_HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation)[_NowLookPage]->SetRotAngle(-90.0f);
			_HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation)[_NowLookPage]->ChangeState(HistoryPage::StateCodeE::Turn);
		}
	}

	//AボタンもしくはJkeyが押されたら。
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A) || KeyBoardInput->isPush(DIK_J))
	{
		//歴史書からリストを取得。
		vector<HistoryPage*> pagelist = _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation);
		//取得したリストサイズが0以上なら削除処理を行う。
		if (pagelist.size() > 0 && 0 <= _NowLookPage  && _NowLookPage < pagelist.size())
		{
			HistoryPage* page = pagelist[_NowLookPage];

			if (page != nullptr)
			{
				_HistoryBook->PutOutPage((LocationCodeE)_NowSelectLocation, page);

				INSTANCE(HistoryManager)->PutOutPage((LocationCodeE)_NowSelectLocation, _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation));

				_IsOperation = false;
				_HistoryBook->SetIsOperation(_IsOperation);
			}
		}
	}
}

/**
* Chip選択中の更新
*/
void HistoryMenu::SelectChipUpdate()
{
	//チップを左右選択.
	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKL))
	{
		_NowSelectChip = min(max(0, _Chip2DList.size() - 1), _NowSelectChip + 1);
	}
	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKR))
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
			INSTANCE(HistoryManager)->SetHistoryChip((LocationCodeE)_NowSelectLocation, _Chip2DList[_NowSelectChip]->GetChipID());
			//搬入したチップを所持チップから削除.
			auto it = _Chip2DList.begin();
			it += _NowSelectChip;
			INSTANCE(GameObjectManager)->AddRemoveList(*it);
			_Chip2DList.erase(it);

			_NowSelectChip = min(max(0, _Chip2DList.size() - 1), _NowSelectChip);
			_NowSelectChip = max(0, _NowSelectChip);

			_IsOperation = false;
			_HistoryBook->SetIsOperation(_IsOperation);
		}
	}
}
