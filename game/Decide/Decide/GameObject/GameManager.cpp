/**
* ゲーム管理クラスの実装.
*/
#include"stdafx.h"
#include"GameManager.h"

/**
* 初期化.
*/
void GameManager::Start()
{
	if (!_Player)
	{
		_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	}
	if (!_StatusWindow)
	{
		_StatusWindow = (StatusWindow*)INSTANCE(GameObjectManager)->FindObject("StatusWindow");
	}
	if (!_HistoryBook)
	{
		_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");
	}
	if (!_PlayerCamera)
	{
		_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
	}
}

/**
* 更新.
*/
void GameManager::Update()
{
	if ((XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_BACK) ||
		KeyBoardInput->isPush(DIK_Q))
		&& !_HistoryBook->GetActive())
	{
		bool active = !_StatusWindow->GetActive();
		if (active)
		{
			_Player->PlayerStopEnable();
			_PlayerCamera->SetIsMove(false);
		}
		else
		{
			_Player->PlayerStopDisable();
			_PlayerCamera->SetIsMove(true);
		}

		_StatusWindow->SetActive(active, true);
	}
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START)
		&& !_StatusWindow->GetActive())
	{
		bool active = !_HistoryBook->GetActive();
		_HistoryBook->SetActive(active, true);
	}
}
