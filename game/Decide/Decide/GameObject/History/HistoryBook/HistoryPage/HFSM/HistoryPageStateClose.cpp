/**
* ���j���̃y�[�W�̕����ԃN���X�̎���.
*/
#include"stdafx.h"
#include"HistoryPageStateClose.h"

#include"..\HistoryPage.h"

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryPageStateClose::Entry()
{
	_BefAngle = _HistoryPage->GetAngle();

	_MoveSpeed = 3.0f;
	_Angle = 0.0f;

	_LerpRate = 0.0f;
}

/**
* �X�V.
*/
void HistoryPageStateClose::Update()
{
	_LerpRate += _MoveSpeed * Time::DeltaTime();
	_LerpRate = min(1.0f, _LerpRate);

	float angle = _LerpRate * _Angle + (1.0f - _LerpRate) * _BefAngle;

	_HistoryPage->Rotation(angle);
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryPageStateClose::Exit()
{
}
