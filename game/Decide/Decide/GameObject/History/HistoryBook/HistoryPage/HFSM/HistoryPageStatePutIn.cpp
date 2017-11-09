/**
* ���j���̃y�[�W�̋��ޏ�ԃN���X�̎���.
*/
#include"stdafx.h"
#include"HistoryPageStatePutIn.h"

#include"..\HistoryPage.h"

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryPageStatePutIn::Entry()
{
	_LerpRate = 0.0f;

	_MoveSpeed = 2.0f;

	_MovePosition = _HistoryPage->GetInitPos();
	_InitialPosition = _MovePosition + Vector3(0, 0, 1);
}

/**
* �X�V.
*/
void HistoryPageStatePutIn::Update()
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

	if (_LerpRate >= 1.0f)
	{
		_HistoryPage->ChangeState(HistoryPage::StateCodeE::Turn);
	}

}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryPageStatePutIn::Exit()
{
	_HistoryPage->SetRotAngle(-90.0f);
}
