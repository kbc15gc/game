//ゲームカメラクラスの実装。

#include "stdafx.h"
#include "FreeCamera.h"

namespace {
	//移動速度。
	const float MOVE_SPEED = 3.0f;
}

//デストラクタ.
FreeCamera::~FreeCamera()
{
	
}

void FreeCamera::Awake()
{
	//カメラコンポーネント
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);

}

void FreeCamera::Start()
{
	transform->SetPosition(Vector3(0, 70, 0));

	//真下を向くように回転。
	transform->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

	_Camera->SetTarget(transform->GetPosition());

	// 最初は更新しない。
	UnActivateFlg();
}

void FreeCamera::UpdateSubClass()
{
	_Move();
}

void FreeCamera::_Move()
{
	//ゲームパッドから取得した方向。
	Vector3 dir = Vector3::zero;
	//コントローラー移動。
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//キーボード(デバッグ用)。
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
	//移動したか。
	if (dir.Length() != 0)
	{
		Vector3 pos;
		pos = transform->GetPosition();
		pos.x += dir.x*MOVE_SPEED;
		pos.z += dir.z*MOVE_SPEED;
		transform->SetPosition(pos);
	}
}