/**
* ゲーム管理クラスの実装.
*/
#include"stdafx.h"
#include"GameManager.h"
#include "GameObject\Village\EventManager.h"

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
	if (VPadInput->IsPush(fbEngine::VPad::ButtonSelect)	&&
		!_HistoryBook->GetActive())
	{
	/*	bool active = !_StatusWindow->GetActive();
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

		_StatusWindow->SetActive(active, true);*/
		if (_Player->GetSpeakFlag() == false)
			INSTANCE(EventManager)->Execute(Event::EventID::StatusWindowA);
	}
	else if (VPadInput->IsPush(fbEngine::VPad::ButtonStart)
		&& !_StatusWindow->GetActive())
	{
		/*bool active = !_HistoryBook->GetActive();
		_HistoryBook->SetActive(active, true);*/
		if (_Player->GetSpeakFlag() == false)
			INSTANCE(EventManager)->Execute(Event::EventID::HistoryBookA);
	}
}
