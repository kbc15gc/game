/**
* 歴史書の待機状態クラスの実装.
*/
#include"stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateIdol.h"

#include"GameObject\Player\Player.h"

/**
* コンストラクタ.
*/
HistoryBookStateIdol::HistoryBookStateIdol(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_GameCamera = (GameCamera*)INSTANCE(GameObjectManager)->FindObject("GameCamera");
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryBookStateIdol::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::Idol, 0.2f, 1);

	_HistoryBook->SetEnable(true);
}

/**
* 更新.
*/
void HistoryBookStateIdol::Update()
{
	if (_GameCamera->GetLerpRate() <= 0.2f)
	{
		_Player->SetEnable(false);
	}
	if (_GameCamera->GetLerpRate() == 0.0f)
	{
		_HistoryBook->ChangeState(HistoryBook::StateCodeE::Open);
	}
}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryBookStateIdol::Exit()
{
}