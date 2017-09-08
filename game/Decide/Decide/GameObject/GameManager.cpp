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
	if (!_StatusWindow)
	{
		_StatusWindow = (StatusWindow*)INSTANCE(GameObjectManager)->FindObject("StatusWindow");
	}
}

/**
* �X�V.
*/
void GameManager::Update()
{
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_BACK))
	{
		bool active = _StatusWindow->GetActive();
		_StatusWindow->SetActive(!active, true);
	}
}
