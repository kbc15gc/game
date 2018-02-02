#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "PlayerCamera.h"

//#ifdef _DEBUG
//�f�X�g���N�^�B
ThirdPersonCamera::~ThirdPersonCamera()
{
	
}

void ThirdPersonCamera::Awake()
{
	GameCamera::Awake();

	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(1.0f);
	_Camera->SetFar(2500.0f);
	_Camera->SetUseTarget(false);
	//_Camera->SetViewAngle(60.0f);
}

void ThirdPersonCamera::Start()
{
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();

	transform->SetPosition(Vector3(_PlayerPos->x, 500.0f, _PlayerPos->z));

	//�^���������悤�ɉ�]�B
	Quaternion rot;
	rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisX,D3DXToRadian(90.0f));
	transform->SetRotation(rot);

	//_Camera->SetTarget(transform->GetPosition() - Vector3(0.0f, 100.0f, 0.0f));
	
	_NowHeight = Camera_Height::Height;
	// �ŏ��͍X�V���Ȃ��B
	UnActivateFlg();
	
}

void ThirdPersonCamera::UpdateSubClass()
{
	//ChangeHeight();

	//Return();

	//���̍����𒲂ׁA����ɉ���������������B
	switch (_NowHeight)
	{
		//����:��B
	//case ThirdPersonCamera::Camera_Height::Low:
		//transform->SetPosition(transform->GetPosition().x, 180.0f, transform->GetPosition().z);

		////�Ⴂ�����̈ړ��X�s�[�h��ݒ�B
		//SetCameraSpeed(_LowCameraSpeed);
		////_Camera->SetNear(170.0f);
		////_Camera->SetFar(190.0f);

		//_Move();
		//break;
		//����:���B
	case ThirdPersonCamera::Camera_Height::Middle:
		transform->SetPosition(transform->GetPosition().x, 500.0f, transform->GetPosition().z);

		//�_�b�V���̃X�s�[�h�ƒ��̍����̈ړ��X�s�[�h��ݒ�B
		DeicideCameraSpeed(_MiddleCameraDashSpeed, _MiddleCameraSpeed);
		//_Camera->SetNear(400.0f);
		//_Camera->SetFar(600.0f);

		_Move();
		break;
		//����:���B
	case ThirdPersonCamera::Camera_Height::Height:
		transform->SetPosition(_HeightPos);
		//_Camera->SetNear(1900.0f);
		//_Camera->SetFar(2100.0f);

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
		pos.x += dir.x*_MoveSpeed * Time::DeltaTime();
		pos.z += dir.z*_MoveSpeed * Time::DeltaTime();
		transform->SetPosition(pos);
	}

	//_Camera->SetTarget(transform->GetPosition() - Vector3(0.0f, transform->GetPosition().y, 0.0f));
}

void ThirdPersonCamera::ChangeHeight()
{
	//�������グ�Ă����B
	if ((KeyBoardInput->isPush(DIK_DOWN)))
	{
		_NowHeight = Add(_NowHeight);
		//if (_NowHeight < Camera_Height::Max) {
		//	transform->SetPosition(transform->GetPosition().x, 0.0f, transform->GetPosition().z);
		//}
	}

	//�����������Ă����B
	if ((KeyBoardInput->isPush(DIK_UP)))
	{
		_NowHeight = Subtract(_NowHeight);
		//if (_NowHeight != Camera_Height::Height) {
		//	transform->SetPosition(transform->GetPosition().x, 0.0f, transform->GetPosition().z);
		//}
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

//#endif // _DEBUG