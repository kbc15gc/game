/**
* ステータス画面クラスの実装.
*/
#include"stdafx.h"
#include"StatusWindow.h"

#include"GameObject\Inventory\Inventory.h"


const int StatusWindow::WindowBackPriorty = 6;

/**
* 初期化.
*/
void StatusWindow::Awake()
{
	//プレイヤークラスを取得.
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	//座標を中心に設定.
	transform->SetPosition(Vector3(g_WindowSize.x / 2, g_WindowSize.y / 2, 0.0f));

	//バックの背景.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", WindowBackPriorty);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	backWindow->SetSize(backWindow->GetSize() * 1.3f);
	backWindow->transform->SetParent(transform);
	backWindow->transform->SetLocalPosition(Vector3::zero);

	//アイテム一覧の背景.
	ImageObject* itemWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", WindowBackPriorty + 1);
	itemWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	itemWindow->SetSize(Vector2(495.0f, 580.0f));
	itemWindow->SetPivot(0.0f, 0.5f);
	itemWindow->transform->SetParent(transform);
	itemWindow->transform->SetLocalPosition(Vector3(0.0f, 47.0f, 0.0f));

	for (int i = 0; i < _WindowCount; i++)
	{
		ItemWindow* iw = INSTANCE(GameObjectManager)->AddNew<ItemWindow>("ItemWindow", WindowBackPriorty + 3);
		iw->transform->SetParent(transform);
		iw->transform->SetLocalPosition(Vector3::zero);
		iw->SetActive(false, true);
		_ItemWindowList.push_back(iw);
	}
	_ItemWindowList[0]->Init(Item::ItemCodeE::Item);
	_ItemWindowList[1]->Init(Item::ItemCodeE::Weapon);
	_ItemWindowList[2]->Init(Item::ItemCodeE::Armor);

	for (int i = 0; i < _WindowCount; i++)
	{
		_ItemWindowList[i]->SetActive((i == _NowSelectWindow), true);
	}

	// お金の表示作成。
	_MoneyFrame = INSTANCE(GameObjectManager)->AddNew<ImageObject>("MoneyFrame", WindowBackPriorty + 2);
	_MoneyFrame->transform->SetParent(transform);
	_MoneyFrame->transform->SetLocalPosition(Vector3(-330.0f, 310.0f, 0.0f));
	_MoneyFrame->SetTexture(LOADTEXTURE("UI/Hotbar.png"));
	_MoneyFrame->SetSize(Vector2(310.0f,52.0f));
	_MoneyRender = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("MoneyRender", WindowBackPriorty+ 3);
	_MoneyRender->transform->SetParent(_MoneyFrame->transform);
	_MoneyRender->transform->SetLocalPosition(Vector3(60.0f,-13.0f,0.0f));
	_MoneyRender->SetParamTextPos(_MoneyRender->GetParamTextPos() + Vector3(-130.0f,0.0f,0.0f));


	//始めは非表示.
	this->SetActive(false, true);
}

/**
* 更新.
*/
void StatusWindow::Update()
{
	int befSelectWindow = _NowSelectWindow;
	_MoneyRender->SetParam("", "UI/coins.png", INSTANCE(Inventory)->GetPlayerMoney(),fbText::TextAnchorE::UpperRight);

	static float ChangeTime = 0.5f;
	static float LocalTime = 0.0f;
	const float RepeatTime = 0.1f;
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
			ChangeTime = RepeatTime;
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
			ChangeTime = RepeatTime;
		}
	}
	else
	{
		ChangeTime = 0.5f;
		LocalTime = 0.0f;
	}

	if (befSelectWindow != _NowSelectWindow)
	{
		SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
		se->Init("Asset/Sound/UI/Menu.wav");
		se->SetDelete(true);
		se->Play(false);

		for (int i = 0; i < _WindowCount; i++)
		{
			_ItemWindowList[i]->SetActive((i == _NowSelectWindow), true);
		}
		static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->DeleteList();
	}

}

void StatusWindow::Execute()
{
	//アクティブ状況を反転にする。
	this->SetActive(!this->_Active, true);

	//効果音再生。
	SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StatusWindow_SE", 0);

	//開いたときと閉じた時で異なる音。
	if (this->_Active)
		se->Init("Asset/Sound/UI/Menu.wav");
	else
		se->Init("Asset/Sound/UI/Menu.wav");

	//再生後に自動で削除するようにする。
	se->SetDelete(true);
	se->Play(false);
}