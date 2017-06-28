#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"

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
	ChangeHeight();

	//���̍����𒲂ׁA����ɉ���������������B
	switch (_NowHeight)
	{
	case ThirdPersonCamera::Camera_Height::Invalid:
		break;
		//����:��B
	case ThirdPersonCamera::Camera_Height::Low:
		transform->SetPosition(transform->GetPosition().x, 180.0f, transform->GetPosition().z);

		//�J�����̈ړ��X�s�[�h��ݒ�B
		_LowCameraSpeed = 9.0f;
		SetCameraSpeed(_LowCameraSpeed);
		Move();
		break;
		//����:���B
	case ThirdPersonCamera::Camera_Height::Middle:
		transform->SetPosition(transform->GetPosition().x, 500.0f, transform->GetPosition().z);

		//�J�����̈ړ��X�s�[�h��ݒ�B
		_MiddleCameraSpeed = 25.0f;
		SetCameraSpeed(_MiddleCameraSpeed);
		Move();
		break;
		//����:���B
	case ThirdPersonCamera::Camera_Height::Height:
		transform->SetPosition(0.0f, 5000.0f,0.0f);
		break;
	default:
		break;
	}
}

void ThirdPersonCamera::Move()
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

void ThirdPersonCamera::ChangeHeight()
{
	if ((KeyBoardInput->isPush(DIK_UP)))
	{
		_NowHeight = Add(_NowHeight);
		if (_NowHeight != Camera_Height::Height) {
			transform->SetPosition(_PlayerPos->x, 0.0f, _PlayerPos->z);
		}
	}

	if ((KeyBoardInput->isPush(DIK_DOWN)))
	{
		_NowHeight = Subtract(_NowHeight);
		if (_NowHeight != Camera_Height::Height) {
			transform->SetPosition(_PlayerPos->x, 0.0f, _PlayerPos->z);
		}
	}
}