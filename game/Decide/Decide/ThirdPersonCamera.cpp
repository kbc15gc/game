#include "stdafx.h"
#include "ThirdPersonCamera.h"

#include "fbEngine\_Object\_Component\_3D\Camera.h"

//デストラクタ。
ThirdPersonCamera::~ThirdPersonCamera()
{
	
}

void ThirdPersonCamera::Awake()
{
	//カメラコンポーネント
	_ThirdPersonCamera = AddComponent<Camera>();
	_ThirdPersonCamera->SetNear(0.01f);
	_ThirdPersonCamera->SetFar(10000.0f);

}

void ThirdPersonCamera::Start()
{
	transform->SetPosition(Vector3(0,5000,0));

	//真下を向くように回転。
	transform->SetRotation(Quaternion(0.75f, 0.0f, 0.0f, 1.0f));

	_ThirdPersonCamera->SetTarget(Vector3(0,0,0));
}

void ThirdPersonCamera::Update()
{
	if ((KeyBoardInput->isPush(DIK_C)))
	{
		if (_ThirdPersonCameraFlag == false)
		{
			_Camera = INSTANCE(GameObjectManager)->mainCamera;

			INSTANCE(GameObjectManager)->mainCamera = _ThirdPersonCamera;
			
			_ThirdPersonCameraFlag = !_ThirdPersonCameraFlag;
		}
		else
		{
			INSTANCE(GameObjectManager)->mainCamera = _Camera;
			//SetCamera(GetCamera());
			_ThirdPersonCameraFlag = !_ThirdPersonCameraFlag;
		}
	}
}

void ThirdPersonCamera::_Move()
{

}