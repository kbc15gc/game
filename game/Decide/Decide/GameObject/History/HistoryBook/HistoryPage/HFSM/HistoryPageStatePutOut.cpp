/**
* ���j���̃y�[�W�̔�����ԃN���X�̎���.
*/
#include"stdafx.h"
#include"HistoryPageStatePutOut.h"

#include"..\HistoryPage.h"

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryPageStatePutOut::Entry()
{
	_BefAngle = _HistoryPage->GetAngle();

	_MoveSpeed = 2.0f;
	_Angle = 0.0f;

	_LerpRate = 0.0f;
}

/**
* �X�V.
*/
void HistoryPageStatePutOut::Update()
{
	_LerpRate += _MoveSpeed * Time::DeltaTime();
	_LerpRate = min(1.0f, _LerpRate);

	float angle = _LerpRate *  _Angle + (1.0f - _LerpRate) *_BefAngle;

	_HistoryPage->Rotation(angle);
	if (_LerpRate >= 1.0f)
	{
		_HistoryPage->ChangeState(HistoryPage::StateCodeE::TakeOff);
	}
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryPageStatePutOut::Exit()
{
}
