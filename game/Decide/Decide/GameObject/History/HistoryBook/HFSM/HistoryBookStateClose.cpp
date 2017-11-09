/**
* ���j���̑ҋ@��ԃN���X�̎���.
*/
#include"stdafx.h"
#include"..\HistoryBook.h"
#include "HistoryBookStateClose.h"

#include"GameObject\Player\Player.h"

namespace
{

	/** ���j���̏o���ʒu. */
	const Vector3 HISTORY_POINT(0.0f, 0.5f, 0.0f);

}

/**
* �R���X�g���N�^.
*/
HistoryBookStateClose::HistoryBookStateClose(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateClose::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::Close, 0.0f, 1);
}

/**
* �X�V.
*/
void HistoryBookStateClose::Update()
{
	if (_HistoryBook->GetIsPlayAnim(HistoryBook::AnimationCodeE::Close) && !_HistoryBook->GetIsPlay())
	{
		_HistoryBook->ChangeState(HistoryBook::StateCodeE::Move);
	}
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryBookStateClose::Exit()
{
	_PlayerFoward = _Player->transform->GetForward();
	Vector3 foward = _PlayerFoward + HISTORY_POINT;
	_HistoryBook->SetDestPos(foward + _Player->transform->GetPosition());
}