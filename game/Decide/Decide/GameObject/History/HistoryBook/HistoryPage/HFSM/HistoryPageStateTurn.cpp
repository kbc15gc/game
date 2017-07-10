/**
* ���j���̃y�[�W�̕����ԃN���X�̎���.
*/
#include"stdafx.h"
#include"HistoryPageStateTurn.h"

#include"..\HistoryPage.h"

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryPageStateTurn::Entry()
{
	_BefAngle = _HistoryPage->GetAngle();

	_MoveSpeed = 2.0f;
	_Angle = _HistoryPage->GetRotAngle();

	_LerpRate = 0.0f;
}

/**
* �X�V.
*/
void HistoryPageStateTurn::Update()
{
	_LerpRate += _MoveSpeed * Time::DeltaTime();
	_LerpRate = min(1.0f, _LerpRate);

	float angle = _LerpRate * _Angle + (1.0f - _LerpRate) * _BefAngle;

	_HistoryPage->Rotation(angle);
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryPageStateTurn::Exit()
{
}
