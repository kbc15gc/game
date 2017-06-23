#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "GameObject\Player\Player.h"

//�f�X�g���N�^�B
ThirdPersonCamera::~ThirdPersonCamera()
{
	
}

void ThirdPersonCamera::Awake()
{
	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);

	_Model= AddComponent<SkinModel>();
}

void ThirdPersonCamera::Start()
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();

	//�^���������悤�ɉ�]�B
	transform->SetRotation(Quaternion(0.75f, 0.0f, 0.0f, 1.0f));

	_Camera->SetTarget(Vector3(0,0,0));

	// �ŏ��͍X�V���Ȃ��B
	UnActivateFlg();

	
}

void ThirdPersonCamera::UpdateSubClass()
{
	if ((KeyBoardInput->isPush(DIK_UP)))
	{
		_NowHeight = Add(_NowHeight);
	}

	if ((KeyBoardInput->isPush(DIK_DOWN)))
	{
		_NowHeight = Subtract(_NowHeight);
	}

	switch (_NowHeight)
	{
	case ThirdPersonCamera::Camera_Height::Invalid:
		break;
	case ThirdPersonCamera::Camera_Height::Low:
		transform->SetPosition(_PlayerPos->x, 80, _PlayerPos->z);
		break;
	case ThirdPersonCamera::Camera_Height::Middle:
		transform->SetPosition(_PlayerPos->x, 500, _PlayerPos->z);
		break;
	case ThirdPersonCamera::Camera_Height::Height:
		transform->SetPosition(_PlayerPos->x, 5000, _PlayerPos->z);
		break;
	default:
		break;
	}
}

void ThirdPersonCamera::_Move()
{

}