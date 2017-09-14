/**
* ���j���̈ړ���ԃN���X�̎���.
*/
#include"stdafx.h"
#include"..\HistoryBook.h"
#include "HistoryBookStateMove.h"

/**
* �R���X�g���N�^.
*/
HistoryBookStateMove::HistoryBookStateMove(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_GameCamera = (GameCamera*)INSTANCE(GameObjectManager)->FindObject("GameCamera");
}

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateMove::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::CloseIdol, 0.0f);
	_HistoryBook->SetIsOperation(false);
	_LerpRate = 0.0f;

	_Position = _HistoryBook->transform->GetPosition();
	_Rotation = _HistoryBook->transform->GetRotation();

}

/**
* �X�V.
*/
void HistoryBookStateMove::Update()
{
	static float SPEED = 0.5f;
	_LerpRate += SPEED * Time::DeltaTime();
	_LerpRate = min(1.0f, _LerpRate);

	Vector3 dest = _HistoryBook->GetDestPos();
	dest.Scale(_LerpRate);
	Vector3 pos = _Position;
	pos.Scale(1.0f - _LerpRate);

	//���W��ݒ�.
	_HistoryBook->transform->SetPosition(dest + pos);

	if (_LerpRate >= 1.0f)
	{
		if (_HistoryBook->GetIsOpenOrClose())
		{
			_HistoryBook->ChangeState(HistoryBook::StateCodeE::Open);
		}
		else
		{
			_HistoryBook->ChangeState(HistoryBook::StateCodeE::Unused);
		}
	}
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryBookStateMove::Exit()
{
	_HistoryBook->SetIsOperation(true);
}