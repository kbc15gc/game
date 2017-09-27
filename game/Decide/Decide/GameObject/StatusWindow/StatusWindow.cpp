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
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 7);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	backWindow->SetSize(backWindow->GetSize() * 1.3f);
	backWindow->transform->SetParent(transform);

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
		iw->transform->SetParent(transform);
		iw->SetActive(false, true);
		_ItemWindowList.push_back(iw);
	}
	_ItemWindowList[0]->Init(Item::ItemCodeE::Item);
	_ItemWindowList[1]->Init(Item::ItemCodeE::Weapon);
	_ItemWindowList[2]->Init(Item::ItemCodeE::Armor);

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
