//�Q�[���J�����N���X�̎����B

#include "stdafx.h"
#include "FreeCamera.h"

namespace {
	//�ړ����x�B
	const float MOVE_SPEED = 5.0f;
}

//�f�X�g���N�^.
FreeCamera::~FreeCamera()
{
	
}

void FreeCamera::Awake()
{
	//�J�����R���|�[�l���g
	_FreeCamera = AddComponent<Camera>();
	_FreeCamera->SetNear(0.01f);
	_FreeCamera->SetFar(10000.0f);

	SetMainCamera();
}

void FreeCamera::Start()
{
	transform->SetPosition(Vector3(0, 80, 0));

	//�^���������悤�ɉ�]�B
	transform->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

	_FreeCamera->SetTarget(transform->GetPosition());
}

void FreeCamera::Update()
{
	if ((KeyBoardInput->isPressed(DIK_V)))
	{
		if (_FreeCameraFlag == false)
		{
			//�ӂ���J��������Q�[���J�����ɖ߂������B
			INSTANCE(GameObjectManager)->mainCamera = _FreeCamera;

			_FreeCameraFlag = !_FreeCameraFlag;
		}
		else
		{
			INSTANCE(GameObjectManager)->mainCamera = _GameCamera;

			_FreeCameraFlag = !_FreeCameraFlag;
		}

	}

	_Move();
}

void FreeCamera::_Move()
{
	//�Q�[���p�b�h����擾��������
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ�
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)
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
	//�ړ�������
	if (dir.Length() != 0)
	{
		Vector3 pos;
		pos = transform->GetPosition();
		pos.x += dir.x*MOVE_SPEED;
		pos.z += dir.z*MOVE_SPEED;
		transform->SetPosition(pos);
	}
}