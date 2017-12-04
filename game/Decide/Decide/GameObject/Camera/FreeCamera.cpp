#include "stdafx.h"
#include "FreeCamera.h"
#include "GameObject\Player\Player.h"
#include "PlayerCamera.h"

#ifdef _DEBUG
//デストラクタ。
FreeCamera::~FreeCamera()
{
	
}

void FreeCamera::Awake()
{
	GameCamera::Awake();

	//カメラコンポーネント
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);

	//定点カメラ１。
	PointCamera PointCamera1;
	PointCamera1.pos = Vector3{ 2600.0f,600.0f,-2500.0f };
	PointCamera1.rot = Vector2(-46.0f, -17.0f);
	_PointCameraInfo.push_back(PointCamera1);

	//定点カメラ２。
	PointCamera PointCamera2;
	PointCamera2.pos = Vector3{ 2600.0f,600.0f,2500.0f };
	PointCamera2.rot = Vector2(-134.0f, -18.2f);
	_PointCameraInfo.push_back(PointCamera2);

	//定点カメラ３。
	PointCamera PointCamera3;
	PointCamera3.pos = Vector3{ -2600.0f,600.0f,2500.0f };
	PointCamera3.rot = Vector2(135.5f, -12.6f);
	_PointCameraInfo.push_back(PointCamera3);

	//定点カメラ４。
	PointCamera PointCamera4;
	PointCamera4.pos = Vector3{ -2600.0f,600.0f,-2500.0f };
	PointCamera4.rot = Vector2(46.0f, -12.6f);
	_PointCameraInfo.push_back(PointCamera4);

}

void FreeCamera::Start()
{
	transform->SetPosition(Vector3(0, 70, 0));

	transform->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

	_Camera->SetTarget(transform->GetPosition());

	// 最初は更新しない。
	UnActivateFlg();
}

void FreeCamera::UpdateSubClass()
{

	//ダッシュか通常時かを見てスピードを決める。
	DeicideCameraSpeed(_DashSpeed, _NormalSpeed);

	//切り替える前のプレイヤーカメラの位置に戻す。
	Return();

	//回転。
	FreeCameraRotation();

	//上下移動。
	FreeCameraMoveUpDown();

	//前後左右の移動。
	_Move();	

	//定点カメラの切り替え。
	ChangePointCamera();
}

void FreeCamera::_Move()
{
	Vector3 movespeed = Vector3::zero;
	//ゲームパッドから取得した方向。
	Vector3 dir = Vector3::zero;
	//コントローラー移動。
	dir.x += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).y / 32767.0f);

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

	//移動したか。
	if (dir.Length() != 0)
	{
		//カメラからみた向きに変換
		Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
		//カメラのビュー行列をゲット
		D3DXMATRIX view = camera->GetViewMat();
		//ビュー行列の逆行列
		D3DXMATRIX viewinv;
		D3DXMatrixInverse(&viewinv, NULL, &view);
		//カメラ空間から見た奥方向のベクトルを取得。
		Vector3 cameraZ;
		cameraZ.x = viewinv.m[2][0];
		cameraZ.y = 0.0f;		//Y軸いらない。
		cameraZ.z = viewinv.m[2][2];
		cameraZ.Normalize();	//Y軸を打ち消しているので正規化する。
								//カメラから見た横方向のベクトルを取得。
		Vector3 cameraX;
		cameraX.x = viewinv.m[0][0];
		cameraX.y = 0.0f;		//Y軸はいらない。
		cameraX.z = viewinv.m[0][2];
		cameraX.Normalize();	//Y軸を打ち消しているので正規化する。

		dir = dir * _MoveSpeed;

		Vector3 pos;
		pos = transform->GetPosition();

		//カメラからみた方向に射影。
		movespeed = movespeed + cameraX * dir.x;
		movespeed.y = dir.y;
		movespeed = movespeed + cameraZ * dir.z;

		transform->SetPosition(pos + movespeed);

		_Camera->SetTarget(Vector3(pos.x, pos.y, pos.z + 1.0f));
	}
}

void FreeCamera::FreeCameraRotation()
{
	//ゲームパッドから入力された値に_CameraRotationSpeedを掛けて回転速度を調整。
	_R_STICK_Y += (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f)*_CameraRotationSpeed;
	_R_STICK_X += (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f)*_CameraRotationSpeed;

	//縦と横に一回転しきったら初期化。
	if (fabs(_R_STICK_X) >= 360)
	{
		_R_STICK_X = 0.0f;
	}
	if (fabs(_R_STICK_Y) >= 360)
	{
		_R_STICK_Y = 0.0f;
	}
	Quaternion q;
	//オイラー角からクォータニオンに変換。
	q.SetEuler(Vector3(_R_STICK_X, -_R_STICK_Y, 0.0f));
	transform->SetRotation(Quaternion(q.y, q.x, q.z, q.w));
}

void FreeCamera::FreeCameraMoveUpDown()
{
	Vector3 pos = transform->GetPosition();
	//右トリガー押し込みで上昇。
	pos.y -= XboxInput(0)->GetAnalog(AnalogE::TRIGGER).x / 255.0f*_MoveSpeed;
	//左トリガー押し込みで下降。
	pos.y += XboxInput(0)->GetAnalog(AnalogE::TRIGGER).y / 255.0f*_MoveSpeed;
	transform->SetPosition(pos);
}

void FreeCamera::Return()
{
	//Pを押したらカメラの位置を変更。
	if (KeyBoardInput->isPush(DIK_P))
	{
		PlayerCamera* playercamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
		//切り替える前のプレイヤーカメラの位置に戻す。
		transform->SetPosition(playercamera->transform->GetPosition());
	}
}

void FreeCamera::ChangePointCamera()
{
	//十字キー右で定点カメラを順繰りに切り替え。
	if ((XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_RIGHT)))
	{

		//添え字が配列分を超えてないかチェック。
		if (_InfoIndex < 4)
		{
			//定点カメラ用の向きと位置を設定。
			_R_STICK_X = _PointCameraInfo[_InfoIndex].rot.x;
			_R_STICK_Y = _PointCameraInfo[_InfoIndex].rot.y;
			transform->SetPosition(_PointCameraInfo[_InfoIndex].pos);

			//添え字を一つずらす。
			_InfoIndex++;
		}
		else
		{
			//添え字が配列分を超えたので配列の一番最初に戻す。
			_InfoIndex = 0;
			_R_STICK_X = _PointCameraInfo[_InfoIndex].rot.x;
			_R_STICK_Y = _PointCameraInfo[_InfoIndex].rot.y;
			transform->SetPosition(_PointCameraInfo[_InfoIndex].pos);
		}
	}
}

#endif // _DEBUG