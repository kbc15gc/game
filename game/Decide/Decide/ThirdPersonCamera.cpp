#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"

//デストラクタ。
ThirdPersonCamera::~ThirdPersonCamera()
{
	
}

void ThirdPersonCamera::Awake()
{
	//カメラコンポーネント
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);

	_Model= AddComponent<SkinModel>();
}

void ThirdPersonCamera::Start()
{
	transform->SetPosition(3378, 5000, -1286);

	//真下を向くように回転。
	transform->SetRotation(Quaternion(0.75f, 0.0f, 0.0f, 1.0f));

	_Camera->SetTarget(Vector3(0,0,0));

	// 最初は更新しない。
	UnActivateFlg();
}

void ThirdPersonCamera::UpdateSubClass()
{
	
}

void ThirdPersonCamera::_Move()
{

}