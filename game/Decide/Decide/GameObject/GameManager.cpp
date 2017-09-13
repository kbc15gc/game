/**
* �Q�[���Ǘ��N���X�̎���.
*/
#include"stdafx.h"
#include"GameManager.h"

/**
* ������.
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
}

/**
* �X�V.
*/
void GameManager::Update()
{
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_BACK)
		&& !_HistoryBook->GetActive())
	{
		bool active = _StatusWindow->GetActive();
		_StatusWindow->SetActive(!active, true);
	}
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START)
		&& !_StatusWindow->GetActive())
	{
		bool active = _HistoryBook->GetActive();
		_HistoryBook->SetActive(!active, true);
	}
}
