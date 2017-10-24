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
	const float CAMERA_SPEED = 3.5f;

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
	//正規化した方向を
	D3DXVECTOR3 baseDir(0.5f, 0.3f, -0.7f);
	D3DXVec3Normalize(&_ToPlayerDir, &baseDir);
	// 初期値設定のため処理を呼ぶ。
	// ※消すな。
	{
		_Camera->SetTarget(_GetPlayerPos());
		transform->SetPosition(_GetPlayerPos() + (_ToPlayerDir * _Dist));
		_Camera->Update();
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
	auto toC = transform->GetPosition() - _GetPlayerPos();
	_ToCamera = D3DXVECTOR3(toC.x, toC.y, toC.z);

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
	_TargetPos = _ClosetRay();

	//プレイヤーを見る。
	_LookAtPlayer();
	//カメラを移動させる。
	_Move();
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
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);

	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;

	D3DXVec3Transform(&v, &_ToCamera, &rot);
	transform->SetPosition(_GetPlayerPos() + Vector3(v.x, v.y, v.z));
}

void PlayerCamera::_RotateVertical(float rotx)
{
	//前の角度を保持。
	D3DXVECTOR3 before = _ToPlayerDir;

	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;

	//外積で直交するベクトルを取得
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &_ToPlayerDir);
	//クォータニオン作成。
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	//回転行列を使って回転させる。
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;

	//正規化する。
	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &_ToPlayerDir);
	bool notlimit = true;
	//カメラの上下の上限
	if (toPosDir.y < -0.5f)
	{
		_ToPlayerDir = before;
		notlimit = false;
	}
	else if (toPosDir.y > 0.8f)
	{
		_ToPlayerDir = before;
		notlimit = false;
	}

	if (notlimit)
	{
		D3DXVec3Transform(&v, &_ToCamera, &rot);
		transform->SetPosition(_GetPlayerPos() + Vector3(v.x, v.y, v.z));
	}
}

Vector3 PlayerCamera::_ClosetRay()
{
	//プレイヤーとカメラの距離
	Vector3 dist = (Vector3)(_ToPlayerDir * _Dist);
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
	if (_IsHitObject = ray.hitObject)
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

void PlayerCamera::_LookAtPlayer()
{

	//Vector3 now = _Camera->GetTarget();
	//Vector3 trg = _GetPlayerPos();
	//auto pos = _CalcSpringDamp(now, trg, _PrevPosition, Time::DeltaTime(), 9.0f, 0.8f, 0.01f);
	//_PrevPosition = trg;
	////プレイヤーの方を向く
	//_Camera->SetTarget(pos);
	//transform->LockAt(pos);

	auto pos = _Camera->GetTarget();
	auto trg = _GetPlayerPos();
	//pos.Lerp(trg, 0.7f);
	_Camera->SetTarget(trg);
	transform->LockAt(trg);
}

void PlayerCamera::_Move()
{
	//補完。
	auto now = transform->GetPosition();
	//移動先計算。
	auto pos = _CalcSpringDamp(now, _TargetPos, _PrevPosition, Time::DeltaTime(), 9.0f, 0.8f, 0.01f);
	_PrevPosition = _TargetPos;
	transform->SetPosition(pos);

	//移動
	/*auto now = transform->GetPosition();
	now.Lerp(_TargetPos, 0.8f);
	transform->SetPosition(now);*/
}

Vector3 PlayerCamera::_CalcSpringDamp(Vector3 curr, Vector3 trgpos, Vector3 prevtrg, float delta, float spring, float damp, float springlen)
{
	Vector3 disp;		//変位。
	Vector3 velocity;	//速度。
	float forceMag;		//力の大きさ。
				   
	//バネの力を計算。
	disp = curr - trgpos;
	if (disp.Length() == 0.0f)
		return curr;
	velocity = (prevtrg - trgpos) * delta;

	forceMag = spring * (springlen - disp.Length()) + damp * (disp.Dot(velocity)) / disp.Length();

	//バネの力を適用。
	disp.Normalize();
	disp = disp * forceMag * delta;
	return curr + disp;
}