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
}

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateIdol::Entry()
{
	if (_HistoryBook->GetIsOpenOrClose())
	{
		_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::OpenIdol, 0.0f);
	}
	else
	{
		_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::CloseIdol, 0.0f);
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