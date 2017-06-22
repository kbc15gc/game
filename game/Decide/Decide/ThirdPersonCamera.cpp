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
	_ThirdPersonCamera = AddComponent<Camera>();
	_ThirdPersonCamera->SetNear(0.01f);
	_ThirdPersonCamera->SetFar(10000.0f);

	SetMainCamera();

}

void ThirdPersonCamera::Start()
{
	transform->SetPosition(Vector3(0,5000,0));

	//�^���������悤�ɉ�]�B
	transform->SetRotation(Quaternion(0.75f, 0.0f, 0.0f, 1.0f));

	_ThirdPersonCamera->SetTarget(Vector3(0,0,0));
}

void ThirdPersonCamera::Update()
{
	if ((KeyBoardInput->isPressed(DIK_C)))
	{
		if (_ThirdPersonCameraFlag == false)
		{
			//�ӂ���J��������Q�[���J�����ɖ߂������B
			INSTANCE(GameObjectManager)->mainCamera = _ThirdPersonCamera;

			_ThirdPersonCameraFlag = !_ThirdPersonCameraFlag;
		}
		else
		{
			INSTANCE(GameObjectManager)->mainCamera = _GameCamera;

			_ThirdPersonCameraFlag = !_ThirdPersonCameraFlag;
		}
		
	}
}

void ThirdPersonCamera::_Move()
{

}