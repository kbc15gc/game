/**
* 歴史変更メニュー画面クラスの実装.
*/
#include"stdafx.h"
#include "HistoryMenu.h"

#include "fbEngine\_Object\_GameObject\TextObject.h"

#include"..\HistoryBook\HistoryBook.h"

#include "GameObject\History\HistoryManager.h"

void HistoryMenu::Awake()
{
	if (IS_CONTINUE)
	{
		LoadChip();
	}
	else
	{
		SaveChip();
	}
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
	_LocationNameRender->SetText(LocationNameList[_NowSelectLocation].c_str());

	_LocationNameRender->SetAnchor(fbText::TextAnchorE::UpperCenter);

	//歴史書のポインタを取得.
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");
	//_HistoryBook->SetNowSelectLocation(_NowSelectLocation);

	_ReleaseLocation = (int)LocationCodeE::Common - 1;

	//スプライトクラスを追加.
	/*_CursorSpriteL = INSTANCE(GameObjectManager)->AddNew<ImageObject>("", 9);
	_CursorSpriteL->SetTexture(LOADTEXTURE("UI/brackets.png"));
	_CursorSpriteL->SetSize(_CursorSpriteL->GetSize() * 0.1f);
	_CursorSpriteR = INSTANCE(GameObjectManager)->AddNew<ImageObject>("", 9);
	_CursorSpriteR->SetTexture(LOADTEXTURE("UI/brackets.png"));
	_CursorSpriteR->SetSize(_CursorSpriteR->GetSize() * 0.1f);
	_CursorSpriteR->transform->SetLocalAngle(0.0f, 0.0f, 180.0f);*/
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
			if (_IsOpen)
			{
				ChipMove();
				_IsOpen = false;
			}
			EnableUpdate();
		}
	}
	else
	{
		//非表示.
		_LocationNameRender->SetActive(false);
		//_CursorSpriteL->SetActive(false);
		//_CursorSpriteR->SetActive(false);
		for (auto& list : _Chip2DList)
		{
			for (auto pChip : list)
			{
				pChip->SetActive(false);
			}
		}

		_IsOpen = true;
	}

	if (_HistoryBook->GetNowState() == (int)HistoryBook::StateCodeE::Idol ||
		_HistoryBook->GetNowState() == (int)HistoryBook::StateCodeE::Open)
	{
		if (_IsOperation)
		{
			//Bボタンで戻る.
			if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
			{
				INSTANCE(EventManager)->Execute(Event::EventID::HistoryBookA);
			}
		}
	}
	/*if (!_IsOperation)
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
	}*/
}

/**
* チップを追加.
*/
void HistoryMenu::AddChip(ChipID chipID, bool isSave)
{
	Chip2D* chip2D = INSTANCE(GameObjectManager)->AddNew<Chip2D>("Chip2D", 9);
	chip2D->Start(chipID);

	int locID = 0;
	switch (chipID)
	{
		case ChipID::Fire:
		case ChipID::Tree:
		case ChipID::Stone:
			locID = 0;
			break;
		case ChipID::Copper:
		case ChipID::Agriculture:
		case ChipID::Hunt:
			locID = 1;
			break;
		case ChipID::Iron:
		case ChipID::Oil:
		case ChipID::Medicine:
			locID = 2;
			break;
		/*case ChipID::DevilTown:
		case ChipID::DevilCastel:
		case ChipID::DevilThrone:
			locID = 3;
			break;*/
	}
	_Chip2DList[locID].push_back(chip2D);

	int size = _Chip2DList[locID].size();
	_NowSelectChip[locID] = min(max(0, size - 1), _NowSelectChip[locID]);
	_NowSelectChip[locID] = max(0, _NowSelectChip[locID]);

	int len = size - 1 - _NowSelectChip[locID];
	float offset = -150.0f;
	Vector3 pos = Vector3((g_WindowSize.x / 2.0f) + (offset * len), g_WindowSize.y - 10.0f, 0.0f);
	chip2D->transform->SetPosition(pos);
	if (size - 1 == _NowSelectChip[locID])
	{
		chip2D->SetSize(Chip2D::SizeCodeE::Select);
	}
	else
	{
		chip2D->SetSize(Chip2D::SizeCodeE::NoSelect);
	}

	ChipMove();

	if (isSave)
	{
		SaveChip();
	}
}

void HistoryMenu::SetLocationCode(LocationCodeE code)
{
	_NowSelectLocation = (int)code;
	_HistoryBook->SetLocationCode((LocationCodeE)_NowSelectLocation);
	_NowLookPage = 0;
	//_SelectCode = (int)SelectCodeE::Page;
}

void HistoryMenu::SetIsOperation(bool value)
{
	_IsOperation = value;
	_HistoryBook->SetIsOperation(_IsOperation);
}

/**
* 表示中の更新.
*/
void HistoryMenu::EnableUpdate()
{
	//Vector2 cursorPos = Vector2((g_WindowSize.x / 2.0f), 0.0f);
	//switch ((SelectCodeE)_SelectCode)
	//{
	//	case SelectCodeE::Location:
	//		//場所選択中の更新.
	//		SelectLocationUpdate();
	//		cursorPos.y = 90.0f;
	//		break;
	//	case SelectCodeE::Page:
	//		//ページ選択中の更新.
	//		SelectPageUpdate();
	//		cursorPos.y = g_WindowSize.y / 2.0f;
	//		break;
	//}

	//ページの初期化.
	SelectPageUpdate();

	if (_IsOperation)
	{
		SelectChipUpdate();
	}

	//static float ChangeTime = 0.5f;
	//static float LocalTime = 0.0f;
	////左スティックの情報.
	//Vector2 LStick = XboxInput(0)->GetAnalog(AnalogE::L_STICK);
	//LStick /= 32767.0f;
	////if (LStick.y >= 0.2f && fabsf(LStick.x) <= 0.1f)
	//if(XboxInput(0)->IsPressAnalog(AnalogE::L_STICKU,true))
	//{
	//	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU))
	//	{
	//		_SelectCode = max((int)SelectCodeE::Min, _SelectCode - 1);
	//	}
	//	LocalTime += Time::DeltaTime();
	//	if (LocalTime >= ChangeTime)
	//	{
	//		_SelectCode = max((int)SelectCodeE::Min, _SelectCode - 1);

	//		LocalTime = 0.0f;
	//		ChangeTime = 0.01f;
	//	}
	//}
	////else if (LStick.y <= -0.2f && fabsf(LStick.x) <= 0.1f)
	//else if(XboxInput(0)->IsPressAnalog(AnalogE::L_STICKD, true))
	//{
	//	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
	//	{
	//		_SelectCode = min((int)SelectCodeE::Max, _SelectCode + 1);
	//	}
	//	LocalTime += Time::DeltaTime();
	//	if (LocalTime >= ChangeTime)
	//	{
	//		_SelectCode = min((int)SelectCodeE::Max, _SelectCode + 1);
	//		LocalTime = 0.0f;
	//		ChangeTime = 0.01f;
	//	}
	//}
	//else
	//{
	//	ChangeTime = 0.5f;
	//	LocalTime = 0.0f;
	//}

	//_CursorSpriteL->transform->SetPosition(Vector3(cursorPos.x, cursorPos.y, 0.0f) + Vector3(-450.0f, 0.0f, 0.0f));
	//_CursorSpriteR->transform->SetPosition(Vector3(cursorPos.x, cursorPos.y, 0.0f) + Vector3(450.0f, 0.0f, 0.0f));

	//表示.
	_LocationNameRender->SetActive(true);
	//_CursorSpriteL->SetActive(true);
	//_CursorSpriteR->SetActive(true);

	//場所名描画.
	_LocationNameRender->SetText(LocationNameList[_NowSelectLocation].c_str());
}

/**
* 場所選択中の更新.
*/
//void HistoryMenu::SelectLocationUpdate()
//{
//
//	//前回の場所.
//	int beforeSelectLocation = _NowSelectLocation;
//
//	static float ChangeTime = 0.5f;
//	static float LocalTime = 0.0f;
//	//左スティックの情報.
//	Vector2 LStick = XboxInput(0)->GetAnalog(AnalogE::L_STICK);
//	LStick /= 32767.0f;
//	if (LStick.x >= 0.2f)
//	{
//		if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKR))
//		{
//			_NowSelectLocation = max(0, _NowSelectLocation - 1);
//		}
//		LocalTime += Time::DeltaTime();
//		if (LocalTime >= ChangeTime)
//		{
//			_NowSelectLocation = max(0, _NowSelectLocation - 1);
//			LocalTime = 0.0f;
//			ChangeTime = 0.01f;
//		}
//	}
//	else if (LStick.x <= -0.2f)
//	{
//		if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKL))
//		{
//			_NowSelectLocation = min(_ReleaseLocation, _NowSelectLocation + 1);
//		}
//		LocalTime += Time::DeltaTime();
//		if (LocalTime >= ChangeTime)
//		{
//			_NowSelectLocation = min(_ReleaseLocation, _NowSelectLocation + 1);
//			LocalTime = 0.0f;
//			ChangeTime = 0.01f;
//		}
//	}
//	else
//	{
//		ChangeTime = 0.5f;
//		LocalTime = 0.0f;
//	}
//
//	if (beforeSelectLocation != _NowSelectLocation)
//	{
//		_NowLookPage = 0;
//		auto& befPageList = _HistoryBook->GetLocationList((LocationCodeE)beforeSelectLocation);
//		if (beforeSelectLocation < _NowSelectLocation)
//		{
//			//現在の場所が前回より大きい数値.
//			for (auto it : befPageList)
//			{
//				it->SetRotAngle(90.0f);
//				it->ChangeState(HistoryPage::StateCodeE::Turn);
//			}
//		}
//		else if (beforeSelectLocation > _NowSelectLocation)
//		{
//			for (auto it : befPageList)
//			{
//				it->SetRotAngle(-90.0f);
//				it->ChangeState(HistoryPage::StateCodeE::Turn);
//			}
//			auto& nowPageList = _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation);
//			for (auto it : nowPageList)
//			{
//				it->SetRotAngle(-90.0f);
//				it->ChangeState(HistoryPage::StateCodeE::Turn);
//			}
//		}
//
//		_HistoryBook->SetLocationCode((LocationCodeE)_NowSelectLocation);
//
//		ChipMove();
//
//		SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
//		se->Init("Asset/Sound/UI/Menu.wav");
//		se->SetDelete(true);
//		se->Play(false);
//	}
//}

/**
* ページ選択中の更新.
*/
void HistoryMenu::SelectPageUpdate()
{
	//現状の見ているページ.
	int beforeLookPage = _NowLookPage;

	static float ChangeTime = 0.5f;
	static float LocalTime = 0.0f;
	//左スティックの情報.
	Vector2 LStick = XboxInput(0)->GetAnalog(AnalogE::L_STICK);
	LStick /= 32767.0f;
	if (LStick.x >= 0.2f)
	{
		int size = _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation).size();
		if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKR))
		{
			_NowLookPage = min(size, _NowLookPage + 1);
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			_NowLookPage = min(size, _NowLookPage + 1);

			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else if (LStick.x <= -0.2f)
	{
		if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKL))
		{
			_NowLookPage = max(0, _NowLookPage - 1);
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			_NowLookPage = max(0, _NowLookPage - 1);
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else
	{
		ChangeTime = 0.5f;
		LocalTime = 0.0f;
	}

	if (beforeLookPage != _NowLookPage)
	{
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

		SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
		se->Init("Asset/Sound/UI/Menu.wav");
		se->SetDelete(true);
		se->Play(false);
	}

	//AボタンもしくはJkeyが押されたら。
	//if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	//{
	//	//歴史書からリストを取得。
	//	vector<HistoryPage*> pagelist = _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation);
	//	//取得したリストサイズが0以上なら削除処理を行う。
	//	if (pagelist.size() > 0 && 0 <= _NowLookPage  && _NowLookPage < pagelist.size())
	//	{
	//		HistoryPage* page = pagelist[_NowLookPage];

	//		if (page != nullptr)
	//		{
	//			_HistoryBook->PutOutPage((LocationCodeE)_NowSelectLocation, page);

	//			INSTANCE(HistoryManager)->PutOutPage((LocationCodeE)_NowSelectLocation, _HistoryBook->GetLocationList((LocationCodeE)_NowSelectLocation));

	//			SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
	//			se->Init("Asset/Sound/UI/paper-tear1.wav");
	//			se->SetDelete(true);
	//			se->Play(false);

	//			PageMove();

	//			_IsOperation = false;
	//			_HistoryBook->SetIsOperation(_IsOperation);
	//		}
	//	}
	//}

}

/**
* Chip選択中の更新
*/
void HistoryMenu::SelectChipUpdate()
{
	int befSelectChip = _NowSelectChip[_NowSelectLocation];

	static float ChangeTime = 0.5f;
	static float LocalTime = 0.0f;

	if (VPadInput->IsPress(fbEngine::VPad::ButtonRB1))
	{
		if (VPadInput->IsPush(fbEngine::VPad::ButtonRB1))
		{
			_NowSelectChip[_NowSelectLocation] = max(0, _NowSelectChip[_NowSelectLocation] - 1);
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			_NowSelectChip[_NowSelectLocation] = max(0, _NowSelectChip[_NowSelectLocation] - 1);
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else if (VPadInput->IsPress(fbEngine::VPad::ButtonLB1))
	{
		if (VPadInput->IsPush(fbEngine::VPad::ButtonLB1))
		{
			_NowSelectChip[_NowSelectLocation] = min(max(0, _Chip2DList[_NowSelectLocation].size() - 1), _NowSelectChip[_NowSelectLocation] + 1);
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			_NowSelectChip[_NowSelectLocation] = min(max(0, _Chip2DList[_NowSelectLocation].size() - 1), _NowSelectChip[_NowSelectLocation] + 1);
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else
	{
		ChangeTime = 0.5f;
		LocalTime = 0.0f;
	}

	if (befSelectChip != _NowSelectChip[_NowSelectLocation])
	{
		ChipMove();

		SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
		se->Init("Asset/Sound/UI/Menu.wav");
		se->SetDelete(true);
		se->Play(false);
	}

	//Aボタン押.
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//存在していれば.
		if (_Chip2DList[_NowSelectLocation].size() != 0 && _Chip2DList[_NowSelectLocation][_NowSelectChip[_NowSelectLocation]] != nullptr)
		{
			//現在指定している場所にチップを設定.
			INSTANCE(HistoryManager)->SetHistoryChip((LocationCodeE)_NowSelectLocation, _Chip2DList[_NowSelectLocation][_NowSelectChip[_NowSelectLocation]]->GetChipID(), _NowLookPage);

			//搬入したチップを所持チップから削除.
			auto it = _Chip2DList[_NowSelectLocation].begin();
			it += _NowSelectChip[_NowSelectLocation];
			INSTANCE(GameObjectManager)->AddRemoveList(*it);
			_Chip2DList[_NowSelectLocation].erase(it);

			SaveChip();

			_NowSelectChip[_NowSelectLocation] = min(max(0, _Chip2DList[_NowSelectLocation].size() - 1), _NowSelectChip[_NowSelectLocation]);
			_NowSelectChip[_NowSelectLocation] = max(0, _NowSelectChip[_NowSelectLocation]);

			ChipMove();

			_IsOperation = false;
			_HistoryBook->SetIsOperation(_IsOperation);
		}
	}

}

void HistoryMenu::ChipMove()
{
	int loc = 0;
	for (auto& list : _Chip2DList)
	{
		for (int i = 0; i < list.size(); i++)
		{
			if (loc == _NowSelectLocation)
			{
				list[i]->SetActive(true, true);
			}
			else
			{
				list[i]->SetActive(false, true);
			}

			int len = i - _NowSelectChip[loc];
			float offset = -150.0f;
			Vector3 pos = Vector3((g_WindowSize.x / 2.0f) + (offset * len), g_WindowSize.y - 10.0f, 0.0f);

			if (i == _NowSelectChip[loc])
			{
				list[i]->SetMove(Chip2D::SizeCodeE::Select, pos);
			}
			else
			{
				list[i]->SetMove(Chip2D::SizeCodeE::NoSelect, pos);
			}
		}
		loc++;
	}
}

/**
* ページの座標移動.
*/
void HistoryMenu::PageMove()
{
	//ページの座標をずらす.
	vector<HistoryPage*> pageList;
	for (auto& loc : _HistoryBook->GetPageList())
	{
		for (auto& page : loc)
		{
			pageList.push_back(page);
		}
	}
	int size = pageList.size();
	for (int i = 0; i < size; i++)
	{
		//位置.
		int pos = i - size / 2;
		Vector3 posVec = Vector3(-0.01f * pos, 0.0f, 0.2f - 0.01f * i);
		pageList[i]->transform->SetLocalPosition(posVec);
	}
}
