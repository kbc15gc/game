#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "PlayerCamera.h"

#ifdef _DEBUG
//�f�X�g���N�^�B
ThirdPersonCamera::~ThirdPersonCamera()
{
	
}

void ThirdPersonCamera::Awake()
{
	GameCamera::Awake();

	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);
}

void ThirdPersonCamera::Start()
{
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

	Return();

	//���̍����𒲂ׁA����ɉ���������������B
	switch (_NowHeight)
	{
		//����:��B
	case ThirdPersonCamera::Camera_Height::Low:
		transform->SetPosition(transform->GetPosition().x, 180.0f, transform->GetPosition().z);

		//�Ⴂ�����̈ړ��X�s�[�h��ݒ�B
		SetCameraSpeed(_LowCameraSpeed);

		_Move();
		break;
		//����:���B
	case ThirdPersonCamera::Camera_Height::Middle:
		transform->SetPosition(transform->GetPosition().x, 500.0f, transform->GetPosition().z);

		//�_�b�V���̃X�s�[�h�ƒ��̍����̈ړ��X�s�[�h��ݒ�B
		DeicideCameraSpeed(_MiddleCameraDashSpeed, _MiddleCameraSpeed);

		_Move();
		break;
		//����:���B
	case ThirdPersonCamera::Camera_Height::Height:
		transform->SetPosition(_HeightPos);
		break;
	default:
		break;
	}
}

void ThirdPersonCamera::_Move()
{
	//�Q�[���p�b�h����擾���������B
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ��B
	dir.x += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).y / 32767.0f);

	//�L�[�{�[�h�B
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
	//�������グ�Ă����B
	if ((KeyBoardInput->isPush(DIK_DOWN)))
	{
		_NowHeight = Add(_NowHeight);
		if (_NowHeight != Camera_Height::Height) {
			transform->SetPosition(transform->GetPosition().x, 0.0f, transform->GetPosition().z);
		}
	}

	//�����������Ă����B
	if ((KeyBoardInput->isPush(DIK_UP)))
	{
		_NowHeight = Subtract(_NowHeight);
		if (_NowHeight != Camera_Height::Height) {
			transform->SetPosition(transform->GetPosition().x, 0.0f, transform->GetPosition().z);
		}
	}
}

void ThirdPersonCamera::Return()
{
	//P����������J�����̈ʒu��ύX�B
	if (KeyBoardInput->isPush(DIK_P))
	{
		PlayerCamera* playercamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
		//�v���C���[������ʒu�ɂӂ���J�������ړ�(X��Z����)�B
		transform->SetPosition(Vector3(playercamera->transform->GetPosition().x, transform->GetPosition().y, playercamera->transform->GetPosition().z));
	}
}

#endif // _DEBUG