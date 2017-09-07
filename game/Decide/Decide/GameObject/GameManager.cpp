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
	if (!_StatusWindow)
	{
		_StatusWindow = (StatusWindow*)INSTANCE(GameObjectManager)->FindObject("StatusWindow");
	}
}

/**
* 更新.
*/
void GameManager::Update()
{
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_BACK))
	{
		bool active = _StatusWindow->GetActive();
		_StatusWindow->SetActive(!active, true);
	}
}
