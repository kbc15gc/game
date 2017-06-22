/**
* ���j���̏�Ԃ̊��N���X�̎���.
*/
#include"stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateUnused.h"

#include"GameObject\Player\Player.h"

/**
* �R���X�g���N�^.
*/
HistoryBookStateUnused::HistoryBookStateUnused(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	_GameCamera = (GameCamera*)INSTANCE(GameObjectManager)->FindObject("GameCamera");
}

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateUnused::Entry()
{
	_HistoryBook->SetEnable(false);
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
	
	_PlayerFoward = _Player->transform->GetForward();
	Vector3 foward = _PlayerFoward;
	_HistoryBook->transform->SetLocalPosition(foward + _Player->transform->GetPosition());

	_HistoryBook->transform->SetRotation(_Player->transform->GetRotation());

	Camera* camera = _GameCamera->GetComponent<Camera>();
	Vector3 cameraFoward = camera->GetTarget() - _GameCamera->transform->GetPosition();
	cameraFoward.Normalize();
	cameraFoward.y -= 0.5f;
	cameraFoward.Scale(0.8f);
	_HistoryBook->SetDestPos(_GameCamera->transform->GetPosition() + cameraFoward);
	
	_HistoryBook->transform->SetRotation(_GameCamera->transform->GetRotation());

	_HistoryBook->SetEnable(true);
}