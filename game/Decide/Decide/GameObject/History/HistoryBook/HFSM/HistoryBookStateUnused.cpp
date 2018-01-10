/**
* ���j���̏�Ԃ̊��N���X�̎���.
*/
#include"stdafx.h"
#include"..\HistoryBook.h"
#include"HistoryBookStateUnused.h"

#include"GameObject\Player\Player.h"

/**
* �R���X�g���N�^.
*/
HistoryBookStateUnused::HistoryBookStateUnused(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
}

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateUnused::Entry()
{
	_HistoryBook->PageInactive();
	_HistoryBook->SetActiveGameObject(false);
	_HistoryBook->PlayerStopDisable();
}

/**
* �X�V.
*/
void HistoryBookStateUnused::Update()
{
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryBookStateUnused::Exit()
{
	Camera* camera = _PlayerCamera->GetComponent<Camera>();


	//�v���C���[����J�����ւ̃x�N�g��.
	Vector3 pToc = _PlayerCamera->transform->GetPosition() - _Player->transform->GetPosition();
	pToc.y = 0.0f;
	pToc.Normalize();

	_HistoryBook->transform->SetLocalPosition(_Player->transform->GetPosition() + pToc + Vector3(0.0f, 1.0f, 0.0f));

	//�J�����̑O����.
	Vector3 cForward = _PlayerCamera->transform->GetForward();
	//�J�����̉�����.
	Vector3 cDown = _PlayerCamera->transform->GetUp() * -1;
	cDown.Scale(0.4f);

	_HistoryBook->SetDestPos(_PlayerCamera->transform->GetPosition() + cForward + cDown);
	
	_HistoryBook->transform->SetRotation(_PlayerCamera->transform->GetRotation());
}