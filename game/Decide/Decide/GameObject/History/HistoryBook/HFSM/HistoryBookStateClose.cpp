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
	_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
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
	Camera* camera = _PlayerCamera->GetComponent<Camera>();

	Vector3 foward = _PlayerCamera->transform->GetPosition() - _Player->transform->GetPosition();
	foward.y = 0.0f;
	foward.Normalize();

	_HistoryBook->SetDestPos(_Player->transform->GetPosition() + foward + Vector3(0.0f, 1.0f, 0.0f));
}