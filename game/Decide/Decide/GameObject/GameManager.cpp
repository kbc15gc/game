/**
* ゲーム管理クラスの実装.
*/
#include"stdafx.h"
#include"GameManager.h"
#include "GameObject\Village\EventManager.h"
#include"GameObject\History\HistoryManager.h"

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
	if (!_worldMap) {
		_worldMap = (WorldMap*)INSTANCE(GameObjectManager)->FindObject("WorldMap");
	}
}

/**
* 更新.
*/
void GameManager::Update()
{
	if (VPadInput->IsPush(fbEngine::VPad::ButtonY)	&&
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
		if (_Player->GetSpeakFlag() == false && (_Player->GetState() != Player::State::Death))
			INSTANCE(EventManager)->Execute(Event::EventID::StatusWindowA);
	}
	else if (VPadInput->IsPush(fbEngine::VPad::ButtonStart)
		&& !_StatusWindow->GetActive()
		&& INSTANCE(HistoryManager)->GetNowLocation() != -1)
	{
		/*bool active = !_HistoryBook->GetActive();
		_HistoryBook->SetActive(active, true);*/
		if (_Player->GetSpeakFlag() == false && (_Player->GetState() != Player::State::Death))
			INSTANCE(EventManager)->Execute(Event::EventID::HistoryBookA);
	}
	else if (VPadInput->IsPush(fbEngine::VPad::ButtonSelect)
		&& !_worldMap->GetActive()/*
		&& INSTANCE(HistoryManager)->GetNowLocation() != -1*/)
	{
		/*bool active = !_HistoryBook->GetActive();
		_HistoryBook->SetActive(active, true);*/
		if (_Player->GetSpeakFlag() == false && (_Player->GetState() != Player::State::Death))
			INSTANCE(EventManager)->Execute(Event::EventID::WorldMapOpen);
	}

	if (_Player->GetState() == Player::State::Death)
	{
		switch (INSTANCE(EventManager)->GetEventID())
		{
			case Event::EventID::StatusWindowA:
				INSTANCE(EventManager)->Execute(Event::EventID::StatusWindowA);
				break;
			case Event::EventID::HistoryBookA:
				INSTANCE(EventManager)->Execute(Event::EventID::HistoryBookA);
				break;
		}
	}
}
