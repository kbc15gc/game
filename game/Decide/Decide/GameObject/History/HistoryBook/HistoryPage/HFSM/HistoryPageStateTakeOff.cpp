/**
* ���j�����甲�����y�[�W�̏�ԃN���X�̎���.
*/
#include "stdafx.h"
#include "HistoryPageStateTakeOff.h"

#include"..\HistoryPage.h"
#include"..\..\..\HistoryManager.h"

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryPageStateTakeOff::Entry()
{
	_LerpRate = 0.0f;

	_MoveSpeed = 2.0f;
}

/**
* �X�V.
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
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryPageStateTakeOff::Exit()
{
}