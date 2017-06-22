/**
* ���j���̑ҋ@��ԃN���X�̎���.
*/
#include"stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateIdol.h"

#include"GameObject\Player\Player.h"

/**
* �R���X�g���N�^.
*/
HistoryBookStateIdol::HistoryBookStateIdol(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateIdol::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::Idol, 0.2f, 1);

	_HistoryBook->SetEnable(true);
}

/**
* �X�V.
*/
void HistoryBookStateIdol::Update()
{
	if (_PlayerCamera->GetLerpRate() <= 0.2f)
	{
		_Player->SetEnable(false);
	}
	if (_PlayerCamera->GetLerpRate() == 0.0f)
	{
		_HistoryBook->ChangeState(HistoryBook::StateCodeE::Open);
	}
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryBookStateIdol::Exit()
{
}