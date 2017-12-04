#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "PlayerCamera.h"

#ifdef _DEBUG
//デストラクタ。
ThirdPersonCamera::~ThirdPersonCamera()
{
	
}

void ThirdPersonCamera::Awake()
{
	GameCamera::Awake();

	//カメラコンポーネント
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);
}

void ThirdPersonCamera::Start()
{
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
	ChangeHeight();

	Return();

	//今の高さを調べ、それに応じた処理をする。
	switch (_NowHeight)
	{
		//高さ:低。
	case ThirdPersonCamera::Camera_Height::Low:
		transform->SetPosition(transform->GetPosition().x, 180.0f, transform->GetPosition().z);

		//低い高さの移動スピードを設定。
		SetCameraSpeed(_LowCameraSpeed);

		_Move();
		break;
		//高さ:中。
	case ThirdPersonCamera::Camera_Height::Middle:
		transform->SetPosition(transform->GetPosition().x, 500.0f, transform->GetPosition().z);

		//ダッシュのスピードと中の高さの移動スピードを設定。
		DeicideCameraSpeed(_MiddleCameraDashSpeed, _MiddleCameraSpeed);

		_Move();
		break;
		//高さ:高。
	case ThirdPersonCamera::Camera_Height::Height:
		transform->SetPosition(_HeightPos);
		break;
	default:
		break;
	}
}

void ThirdPersonCamera::_Move()
{
	//ゲームパッドから取得した方向。
	Vector3 dir = Vector3::zero;
	//コントローラー移動。
	dir.x += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).y / 32767.0f);

	//キーボード。
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

void ThirdPersonCamera::ChangeHeight()
{
	//高さを上げていく。
	if ((KeyBoardInput->isPush(DIK_DOWN)))
	{
		_NowHeight = Add(_NowHeight);
		if (_NowHeight != Camera_Height::Height) {
			transform->SetPosition(transform->GetPosition().x, 0.0f, transform->GetPosition().z);
		}
	}

	//高さを下げていく。
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
	//Pを押したらカメラの位置を変更。
	if (KeyBoardInput->isPush(DIK_P))
	{
		PlayerCamera* playercamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
		//プレイヤーがいる位置にふかんカメラを移動(XとZだけ)。
		transform->SetPosition(Vector3(playercamera->transform->GetPosition().x, transform->GetPosition().y, playercamera->transform->GetPosition().z));
	}
}

#endif // _DEBUG