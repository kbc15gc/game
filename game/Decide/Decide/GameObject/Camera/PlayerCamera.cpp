#include"stdafx.h"
#include "PlayerCamera.h"

#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"

namespace
{
	/** プレイヤーの高さ. */
	const Vector3 PLAYER_HEIGHT(0.0f, 1.5f, 0.0f);
	/** 回転速度. */
	const float CAMERA_SPEED = 2.0f;
}

PlayerCamera::PlayerCamera(const char * name) :
	GameCamera(name),
	_Spring(50.0f),
	_Damping(12.0f)
{
}

/**
* デストラクタ.
*/
PlayerCamera::~PlayerCamera()
{
	//シャドウマップに設定したカメラを外す
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(nullptr);
}

void PlayerCamera::Awake()
{
	GameCamera::Awake();

	//カメラコンポーネント
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;

	//カメラのコリジョンの半径設定
	_Radius = 0.001f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//距離の下限と上限
	_Dist = 5.0f;

}

void PlayerCamera::Start()
{
	//プレイヤーのポジションへの参照を取得
	_PlayerPos = &_Player->transform->GetPosition();
	// 初期値設定のため処理を呼ぶ。
	// ※消すな。
	{
		_Camera->SetTarget(_GetPlayerPos());
		_DestinationPos = _GetPlayerPos() + (_Player->transform->GetForward() * _Dist);
		transform->SetPosition(_DestinationPos);
		_Camera->Update();

		_ToCameraDir = (_Player->transform->GetForward());
	}

	//歴史書を検索。
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	//カメラをシャドウマップに設定.
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(_Camera);
}

void PlayerCamera::UpdateSubClass()
{
	float Pint = 3.0f;

	//歴史書を見ているかどうか。
	if (_IsMove)
	{
		_StandardBehavior();
		Pint = 3.0f;
	}
	else
	{
		Vector3 CameraToHistory;
		CameraToHistory.Subtract(_HistoryBook->transform->GetPosition(), transform->GetPosition());
		Pint = min(3.0f, CameraToHistory.Length());
	}

	INSTANCE(SceneManager)->GetDepthofField().SetPint(Pint * 1000);
	INSTANCE(SceneManager)->GetDepthofField().SetFParam(5.6f);
	INSTANCE(SceneManager)->GetDepthofField().SetFocalLength(24.0f);
}

Vector3 PlayerCamera::_GetPlayerPos()
{
	return (*_PlayerPos) + PLAYER_HEIGHT;
}

/**
* 通常時のカメラ挙動.
*/
void PlayerCamera::_StandardBehavior()
{
	//プレイヤーを見る。
	_LookAtPlayer();

	//右回転
	if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) > 0.1f)
	{
		_RotateHorizon(CAMERA_SPEED * Time::DeltaTime());
	}
	//左回転
	if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) < -0.1f)
	{
		_RotateHorizon(-CAMERA_SPEED * Time::DeltaTime());
	}
	//上
	if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) > 0.1f)
	{
		_RotateVertical(CAMERA_SPEED * Time::DeltaTime());
	}
	//下
	if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) < -0.1f)
	{
		_RotateVertical(-CAMERA_SPEED * Time::DeltaTime());
	}

	//移動先ポジションを取得。
	_DestinationPos = _ClosetRay();

	//カメラを移動させる。
	_Move();
}

void PlayerCamera::_LookAtPlayer()
{
	auto trg = _GetPlayerPos();
	_Camera->SetTarget(trg);
	transform->LockAt(trg);
}

void PlayerCamera::_RotateHorizon(float roty)
{
	D3DXQUATERNION mAxisY;
	D3DXMATRIX rot;
	//Y軸周りにroty度回転するクォータニオンを作成。
	D3DXQuaternionRotationAxis(&mAxisY, &(const D3DXVECTOR3&)Vector3::up, roty);
	//クォータニオンから回転行列を作成。
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);

	//ベクトルを回転行列を使い回転させる。
	D3DXVECTOR4 v;
	D3DXVec3Transform(&v, &(D3DXVECTOR3)_ToCameraDir, &rot);
	_ToCameraDir = v;
	auto nextPos = _GetPlayerPos() + (Vector3(v.x, v.y, v.z) * _Dist);
	//transform->SetPosition(nextPos);
	//_DestinationPos = nextPos;
}

void PlayerCamera::_RotateVertical(float rotx)
{
	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;

	//外積で直交するベクトルを取得
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &(D3DXVECTOR3)_ToCameraDir);
	//クォータニオン作成。
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	//回転行列を使って回転させる。
	D3DXVec3Transform(&v, &(D3DXVECTOR3)_ToCameraDir, &rot);
	
	//正規化する。
	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &D3DXVECTOR3(v.x, v.y, v.z));

	bool notlimit = true;
	//カメラの上下の上限
	if (rotx > 0.0f && toPosDir.y < -0.5f)
	{
		notlimit = false;
	}
	else if (rotx < 0.0f && toPosDir.y > 0.8f)
	{
		notlimit = false;
	}

	if (notlimit)
	{
		_ToCameraDir = v;
		auto nextPos = _GetPlayerPos() + (Vector3(v.x, v.y, v.z) * _Dist);
		//transform->SetPosition(nextPos);
		//_DestinationPos = nextPos;
	}
}

Vector3 PlayerCamera::_ClosetRay()
{
	//プレイヤーとカメラの距離
	Vector3 dist = (Vector3)(_ToCameraDir * _Dist);
	Vector3 from, to;
	//注視点
	from = _Camera->GetTarget();
	//カメラの移動先
	to = from + dist;

	// 衝突を無視する属性を設定。
	int attri = (Collision_ID::ATTACK) | (Collision_ID::PLAYER) | (Collision_ID::ENEMY) | (Collision_ID::BOSS) | (Collision_ID::NOTHITCAMERA) | (Collision_ID::CHARACTER_GHOST);
	//レイを飛ばす
	auto ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere, from, to, attri);

	//レイが何かに当たったかどうか？
	if (ray.hitObject)
	{
		//衝突点の法線方向に半径分移動するベクトル。
		Vector3 normal = ray.hitNormal;
		normal.Scale(_Radius);
		//衝突点から法線の方向に押し出し、移動先を算出。
		return ray.hitPos + normal;
	}
	else
	{
		//どこにも当たらなかったので、レイの終点を返す。
		return to;
	}
}

void PlayerCamera::_Move()
{
	//バネカメラ
	_SpringChaseMove(Time::DeltaTime());
}

void PlayerCamera::_SpringChaseMove(float time)
{
	auto& CameraPos = transform->GetPosition();
	auto& TargetPos = _Camera->GetTarget();
	//目標に対するカメラの現在の位置を基に理想方位角を更新。
	//auto ang = atan2f(pos.y - trg.y, pos.x - trg.x);
	//ワールド行列でのカメラの理想位置。
	auto vIdealPos = _DestinationPos;

	//この理想位置へのバネによる加速度を計算。
	auto vDisplace = CameraPos - vIdealPos;
	auto vSpringAccel = (-_Spring * vDisplace) - (_Damping * _Velocity);
	//オイラー積分を使ってカメラの速度と位置を更新。
	_Velocity += vSpringAccel * time * CAMERA_SPEED;
	transform->SetPosition(CameraPos + (_Velocity * time));
}