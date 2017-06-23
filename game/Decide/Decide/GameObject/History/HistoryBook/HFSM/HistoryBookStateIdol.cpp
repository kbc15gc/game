/**
* ���j���̑ҋ@��ԃN���X�̎���.
*/
#include"stdafx.h"
#include"..\HistoryBook.h"
#include "HistoryBookStateIdol.h"

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
	if (_HistoryBook->GetIsOpenOrClose())
	{
		_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::OpenIdol, 0.1f);
	}
	else
	{
		_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::CloseIdol, 0.1f);
	}
}

/**
* �X�V.
*/
void HistoryBookStateIdol::Update()
{

}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryBookStateIdol::Exit()
{

}