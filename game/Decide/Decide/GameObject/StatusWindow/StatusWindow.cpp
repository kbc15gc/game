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

	int ParamCount = 7;
	for (int i = 0; i < ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}
	_ParameterRenderList[0]->SetParam("LV", "UI/gem.png", _Player->GetParamPt(CharacterParameter::Param::LV));
	_ParameterRenderList[1]->SetParam("EXP", "UI/S_Light01.png", _Player->GetExpPt());
	_ParameterRenderList[2]->SetParam("HP", "UI/hp.png", _Player->GetParamPt(CharacterParameter::Param::HP), reinterpret_cast<int*>(_Player->GetMaxHPPt()));
	_ParameterRenderList[3]->SetParam("MP", "UI/mp.png", _Player->GetParamPt(CharacterParameter::Param::MP), reinterpret_cast<int*>(_Player->GetMaxHPPt()));
	_ParameterRenderList[4]->SetParam("ATK", "UI/S_Buff02.png", _Player->GetParamPt(CharacterParameter::Param::ATK));
	_ParameterRenderList[5]->SetParam("DEF", "UI/S_Buff03.png", _Player->GetParamPt(CharacterParameter::Param::DEF));
	_ParameterRenderList[6]->SetParam("MONEY", "UI/coins.png", INSTANCE(Inventory)->GetPlayerMoneyPt());

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

	static float ChangeTime = 0.5f;
	static float LocalTime = 0.0f;
	//左スティックの情報.
	Vector2 LStick = XboxInput(0)->GetAnalog(AnalogInputE::L_STICK);
	LStick /= 32767.0f;
	if (LStick.x <= -0.2f)
	{
		if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKL))
		{
			_NowSelectWindow = max(0, _NowSelectWindow - 1);
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			_NowSelectWindow = max(0, _NowSelectWindow - 1);
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else if (LStick.x >= 0.2f)
	{
		if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKR))
		{
			_NowSelectWindow = min(_WindowCount - 1, _NowSelectWindow + 1);
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			_NowSelectWindow = min(_WindowCount - 1, _NowSelectWindow + 1);
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
