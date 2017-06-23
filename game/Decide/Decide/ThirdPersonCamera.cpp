#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "GameObject\Player\Player.h"

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
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//プレイヤーのポジションへの参照を取得
	_PlayerPos = &_Player->transform->GetPosition();

	//真下を向くように回転。
	transform->SetRotation(Quaternion(0.75f, 0.0f, 0.0f, 1.0f));

	_Camera->SetTarget(Vector3(0,0,0));

	// 最初は更新しない。
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

	//今の高さを調べ、それに応じた処理をする。
	switch (_NowHeight)
	{
	case ThirdPersonCamera::Camera_Height::Invalid:
		break;
		//高さ:低。
	case ThirdPersonCamera::Camera_Height::Low:
		transform->SetPosition(transform->GetPosition().x, 80, transform->GetPosition().z);

		//カメラの移動スピードを設定。
		SetCameraSpeed(3.0f);
		Move();
		break;
		//高さ:中。
	case ThirdPersonCamera::Camera_Height::Middle:
		transform->SetPosition(transform->GetPosition().x, 500, transform->GetPosition().z);

		//カメラの移動スピードを設定。
		SetCameraSpeed(8.0f);
		Move();
		break;
		//高さ:高。
	case ThirdPersonCamera::Camera_Height::Height:
		transform->SetPosition(transform->GetPosition().x, 5000, transform->GetPosition().z);
		break;
	default:
		break;
	}
}

void ThirdPersonCamera::Move()
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
		pos.x += dir.x*_MoveSpeed;
		pos.z += dir.z*_MoveSpeed;
		transform->SetPosition(pos);
	}
}