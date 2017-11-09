/**
* 歴史書から抜いたページの状態クラスの実装.
*/
#include "stdafx.h"
#include "HistoryPageStateTakeOff.h"

#include"..\HistoryPage.h"
#include"..\..\..\HistoryManager.h"

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryPageStateTakeOff::Entry()
{
	_LerpRate = 0.0f;

	_MoveSpeed = 2.0f;
}

/**
* 更新.
*/
void HistoryPageStateTakeOff::Update()
{
	_LerpRate += _MoveSpeed * Time::DeltaTime();

	float lerpRate = min(1.0f, _LerpRate);

	Vector3 pos = Vector3::zero;
	Vector3 movePos = _MovePosition;
	movePos.Scale(lerpRate);
	Vector3 initPos = _InitialPosition;
	initPos.Scale((1.0f - lerpRate));

	pos.Add(movePos, initPos);

	_HistoryPage->transform->SetLocalPosition(pos);

	if (lerpRate >= 1.0f)
	{
		INSTANCE(HistoryManager)->AddPossessionChip(_HistoryPage->GetChipID());
		INSTANCE(GameObjectManager)->AddRemoveList(_HistoryPage);
	}
}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryPageStateTakeOff::Exit()
{
}