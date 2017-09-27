/**
* ステータス画面クラスの実装.
*/
#include"stdafx.h"
#include"StatusWindow.h"

#include"GameObject\Inventory\Inventory.h"

/**
* 初期化.
*/
void StatusWindow::Start()
{
	//プレイヤークラスを取得.
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	//座標を中心に設定.
	transform->SetPosition(Vector3(g_WindowSize.x / 2, g_WindowSize.y / 2, 0.0f));

	//バックの背景.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 9);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	backWindow->SetSize(backWindow->GetSize() * 1.3f);
	backWindow->transform->SetParent(transform);

	// ステータス表示作成。
	_CreateShowStatus();

	//アイテム一覧の背景.
	ImageObject* itemWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 9);
	itemWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	itemWindow->SetSize(Vector2(495.0f, 580.0f));
	itemWindow->SetPivot(0.0f, 0.5f);
	itemWindow->transform->SetParent(transform);
	itemWindow->transform->SetLocalPosition(Vector3(0.0f, 47.0f, 0.0f));

	for (int i = 0; i < _WindowCount; i++)
	{
		ItemWindow* iw = INSTANCE(GameObjectManager)->AddNew<ItemWindow>("ItemWindow", 9);
		iw->transform->SetParent(itemWindow->transform);
		iw->SetActive(false, true);
		_ItemWindowList.push_back(iw);
	}
	_ItemWindowList[0]->Init(Item::ItemCodeE::Weapon, "武器一覧");
	_ItemWindowList[1]->Init(Item::ItemCodeE::Armor, "防具一覧");
	_ItemWindowList[2]->Init(Item::ItemCodeE::Item, "アイテム一覧");

	//始めは非表示.
	this->SetActive(false, true);
}

/**
* 更新.
*/
void StatusWindow::Update()
{
	_ConfigParamRender();

	static float ChangeTime = 0.5f;
	static float LocalTime = 0.0f;
	if (XboxInput(0)->IsPressButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			if (_NowSelectWindow == 0)
			{
				_NowSelectWindow = _WindowCount - 1;
			}
			else
			{
				_NowSelectWindow -= 1;
			}
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			if (_NowSelectWindow == 0)
			{
				_NowSelectWindow = _WindowCount - 1;
			}
			else
			{
				_NowSelectWindow -= 1;
			}
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else if (XboxInput(0)->IsPressButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			if (_NowSelectWindow == _WindowCount - 1)
			{
				_NowSelectWindow = 0;
			}
			else
			{
				_NowSelectWindow += 1;
			}
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			if (_NowSelectWindow == _WindowCount - 1)
			{
				_NowSelectWindow = 0;
			}
			else
			{
				_NowSelectWindow += 1;
			}
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else
	{
		ChangeTime = 0.5f;
		LocalTime = 0.0f;
	}

	for (int i = 0; i < _WindowCount; i++)
	{
		_ItemWindowList[i]->SetActive((i == _NowSelectWindow), true);
	}
}


void StatusWindow::_CreateShowStatus()
{
	ShowStatus ParamCount = ShowStatus::MAX;
	for (int i = 0; i < ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}
	// レベルのパラメータは真横に表示。
	_ParameterRenderList[ShowStatus::LV]->SetParamTextPos(_ParameterRenderList[ShowStatus::LV]->GetParamTextPos() + Vector3(-200, 0.0f, 0.0f));

	_ExpBar = AddComponent<ParameterBar>();
	vector<BarColor> barColor;
	barColor.push_back(BarColor::Yellow);
	_ExpBar->Create(barColor, static_cast<float>(_Player->GetNextLevelExp()), static_cast<float>(_Player->GetExp()), false,false, _ParameterRenderList[ShowStatus::LV]->transform, Vector3(50.0f, 0.0f, 0.0f), Vector2::one,9);

	_HpBar = AddComponent<ParameterBar>();
	barColor.clear();
	barColor.push_back(BarColor::Green);
	_HpBar->Create(barColor, static_cast<float>(_Player->GetMaxHP()), static_cast<float>(_Player->GetParam(CharacterParameter::Param::HP)), false,false, _ParameterRenderList[ShowStatus::HP]->transform, Vector3(50.0f, 0.0f, 0.0f), Vector2::one,9);

	_MpBar = AddComponent<ParameterBar>();
	barColor.clear();
	barColor.push_back(BarColor::Blue);
	_MpBar->Create(barColor, static_cast<float>(_Player->GetMaxMP()), static_cast<float>(_Player->GetParam(CharacterParameter::Param::MP)),false, false, _ParameterRenderList[ShowStatus::MP]->transform, Vector3(50.0f, 0.0f, 0.0f), Vector2::one,9);

	_ConfigParamRender();
}


void StatusWindow::_ConfigParamRender() {
	int playerLevel = _Player->GetParam(CharacterParameter::Param::LV);
	_ParameterRenderList[ShowStatus::LV]->SetParam("LV", "UI/gem.png", playerLevel);
	_ParameterRenderList[ShowStatus::HP ]->SetParam("HP", "UI/hp.png", _Player->GetParam(CharacterParameter::Param::HP), 0, _Player->GetMaxHP());
	_ParameterRenderList[ShowStatus::MP]->SetParam("MP", "UI/mp.png", _Player->GetParam(CharacterParameter::Param::MP), 0, _Player->GetMaxMP());
	_ParameterRenderList[ShowStatus::ATK]->SetParam("ATK", "UI/S_Buff02.png", _Player->GetParam(CharacterParameter::Param::ATK), _Player->GetBuffParam(CharacterParameter::Param::ATK) - _Player->GetDebuffParam(CharacterParameter::Param::ATK));
	_ParameterRenderList[ShowStatus::MAT]->SetParam("MAT", "UI/S_Buff02.png", _Player->GetParam(CharacterParameter::Param::MAT), _Player->GetBuffParam(CharacterParameter::Param::MAT) - _Player->GetDebuffParam(CharacterParameter::Param::MAT));
	_ParameterRenderList[ShowStatus::DEF]->SetParam("DEF", "UI/S_Buff03.png", _Player->GetParam(CharacterParameter::Param::DEF), _Player->GetBuffParam(CharacterParameter::Param::DEF) - _Player->GetDebuffParam(CharacterParameter::Param::DEF));
	_ParameterRenderList[ShowStatus::MDE]->SetParam("MDE", "UI/S_Buff03.png", _Player->GetParam(CharacterParameter::Param::MDE), _Player->GetBuffParam(CharacterParameter::Param::MDE) - _Player->GetDebuffParam(CharacterParameter::Param::MDE));
	_ParameterRenderList[ShowStatus::DEX]->SetParam("DEX", "UI/S_Buff02.png", _Player->GetParam(CharacterParameter::Param::DEX), _Player->GetBuffParam(CharacterParameter::Param::DEX) - _Player->GetDebuffParam(CharacterParameter::Param::DEX));
	_ParameterRenderList[ShowStatus::MONEY]->SetParam("MONEY", "UI/coins.png", INSTANCE(Inventory)->GetPlayerMoney());

	if (playerLevel != _playerLevel) {
		// レベル変動。
		_playerLevel = playerLevel;
		_ExpBar->Reset(static_cast<float>(_Player->GetNextLevelExp()), static_cast<float>(_Player->GetExp()),false);
		_HpBar->Reset(static_cast<float>(_Player->GetMaxHP()), static_cast<float>(_Player->GetParam(CharacterParameter::HP)), false);
		_MpBar->Reset(static_cast<float>(_Player->GetMaxMP()), static_cast<float>(_Player->GetParam(CharacterParameter::MP)), false);
	}
	else {
		_ExpBar->SetValue(static_cast<float>(_Player->GetExp()));
		_HpBar->SetValue(static_cast<float>(_Player->GetParam(CharacterParameter::HP)));
		_MpBar->SetValue(static_cast<float>(_Player->GetParam(CharacterParameter::MP)));
	}
}