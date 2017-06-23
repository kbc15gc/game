#include "stdafx.h"
#include "FreeCamera.h"
#include "GameObject\Player\Player.h"
//�f�X�g���N�^.
FreeCamera::~FreeCamera()
{
	
}

void FreeCamera::Awake()
{
	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);

	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

void FreeCamera::Start()
{
	transform->SetPosition(Vector3(0, 70, 0));

	//�^���������悤�ɉ�]�B
	transform->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

	_Camera->SetTarget(transform->GetPosition());

	// �ŏ��͍X�V���Ȃ��B
	UnActivateFlg();
}

void FreeCamera::UpdateSubClass()
{
	Move();
}

void FreeCamera::Move()
{
	//�Q�[���p�b�h����擾���������B
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ��B
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)�B
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z++;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z--;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x--;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x++;
	}
#endif
	//�ړ��������B
	if (dir.Length() != 0)
	{
		Vector3 pos;
		pos = transform->GetPosition();
		pos.x += dir.x*_MoveSpeed;
		pos.z += dir.z*_MoveSpeed;
		transform->SetPosition(pos);
	}
}