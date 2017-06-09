#include"stdafx.h"
#include "GameCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\Player\Player.h"
#include "GameObject\HistoryBook\HistoryBook.h"

namespace
{
	const Vector3 PLAYER_HEIGHT(0.0f, 1.5f, 0.0f);
	const float CAMERA_SPEED = 1.0f;
}

void GameCamera::Awake()
{
	//カメラコンポーネント
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;

	//カメラのコリジョンの半径設定
	_Radius = 0.5f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//距離の下限と上限
	_Dist = 7.0f;

	//線形補間の比率を初期化。
	_LerpRate = 1.0f;
}

void GameCamera::Start()
{
	//プレイヤーを検索
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//プレイヤーのポジションへの参照を取得
	_PlayerPos = &_Player->transform->GetPosition();
	//正規化した方向を入れる
	D3DXVec3Normalize(&_ToPlayerDir, &D3DXVECTOR3(0.0f, 3.0f, -4.0f));

	//歴史書を検索。
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	//カメラをシャドウマップに設定.
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(_Camera);
}

void GameCamera::Update()
{
	//歴史書を見ているかどうか。
	if (_HistoryBook->GetIsLookAtHistoryFlag() == false)
	{
		_StandardBehavior();
	}
	else
	{
		_HistoryBehavior();
	}

	_toPosition.Subtract(transform->GetPosition(), _Camera->GetTarget());

	float Len = _toPosition.Length();

	INSTANCE(SceneManager)->GetDepthofField().SetPint(Len * 1000);
	INSTANCE(SceneManager)->GetDepthofField().SetFParam(5.6f);
	INSTANCE(SceneManager)->GetDepthofField().SetFocalLength(26.0f);

}

void GameCamera::RotTransversal(float roty)
{
	D3DXQUATERNION mAxisY;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	D3DXQuaternionRotationAxis(&mAxisY, &(const D3DXVECTOR3&)Vector3::up, roty);
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;
}

void GameCamera::RotLongitudinal(float rotx)
{
	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	//外積で直交するベクトルを取得
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &_ToPlayerDir);
	//
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	D3DXVECTOR3 oldDir = _ToPlayerDir;
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;

	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &_ToPlayerDir);
	//カメラの上下の上限
	if (toPosDir.y < -0.5f)
	{
		_ToPlayerDir = oldDir;
	}
	else if (toPosDir.y > 0.8f)
	{
		_ToPlayerDir = oldDir;
	}
}

void GameCamera::_Move()
{
	//プレイヤーとカメラの距離
	Vector3 dist = (Vector3)(_ToPlayerDir * _Dist);
	Vector3 from, to;
	//注視点
	from = (*_PlayerPos) + PLAYER_HEIGHT;
	//カメラの移動先
	to = from + dist;
	//レイを飛ばす
	int attri = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::ATTACK);	// 衝突を無視する属性を減算。
	fbPhysicsCallback::ClosestConvexResultCallback ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere,from, to, attri);
	//移動先ポジション
	Vector3 next;
	//レイが何かに当たったかどうか？
	if(ray.hitObject)
	{
		//衝突点の法線方向に半径分移動するベクトル。
		Vector3 normal = ray.hitNormal;
		normal.Scale(_Radius);
		//衝突点から法線の方向に押し出し、移動先を算出。
		next = ray.hitPos + normal;
	}
	else
	{
		//どこにも当たらなかったので、レイの終点に移動する。
		next = to;
	}
	//移動
	transform->SetPosition(next);
}

void GameCamera::_StandardBehavior()
{

	//補間の比率を加算。
	//線形補間中。
	if (_LerpRate < 1.0f)
	{
		_LerpRate += 0.01f;

		//カメラの注視点の線形補間を行う(歴史書からプレイヤーに向けて補間)。
		_LerpCameraLookAtPos = (_HistoryBookPos* (1.0f - _LerpRate) + ((*_PlayerPos) + PLAYER_HEIGHT) * _LerpRate);

		//線形補間を行う(プレイヤーの位置から歴史書を見始めた時のカメラの位置に向けて補間)。
		_LerpCameraPos = (_LerpCameraPos * (1.0f - _LerpRate) + _PrevGameCameraPos * _LerpRate);

		//カメラの注視点を線形補間された位置に設定。
		transform->LockAt((_LerpCameraLookAtPos));
		_Camera->SetTarget(_LerpCameraLookAtPos);


		//カメラの位置を線形補完された位置に設定。
		transform->SetPosition((_LerpCameraPos));
	}
	//線形補間をし終わったので通常のカメラの動きをする。
	else
	{
		//右回転
		if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).x / 32767.0f) > 0.1f)
		{
			RotTransversal(CAMERA_SPEED * Time::DeltaTime());
		}
		//左回転
		if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).x / 32767.0f) < -0.1f)
		{
			RotTransversal(-CAMERA_SPEED * Time::DeltaTime());
		}
		//上
		if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).y / 32767.0f) > 0.1f)
		{
			RotLongitudinal(CAMERA_SPEED * Time::DeltaTime());
		}
		//下
		if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).y / 32767.0f) < -0.1f)
		{
			RotLongitudinal(-CAMERA_SPEED * Time::DeltaTime());
		}

		//移動
		_Move();

		//歴史書を見始めた時点でのゲームカメラの位置を確保。
		_PrevGameCameraPos = this->transform->GetPosition();

		_LerpRate = 1.0f;
	}

	//プレイヤーの方を向く
	transform->LockAt((*_PlayerPos) + PLAYER_HEIGHT);
	_Camera->SetTarget((*_PlayerPos) + PLAYER_HEIGHT);

}

void GameCamera::_HistoryBehavior()
{
	//歴史書の位置を取得。
	_HistoryBookPos = _HistoryBook->transform->GetPosition();

	//補間の比率を減算。
	if (_LerpRate > 0.0f)
	{
		_LerpRate -= 0.01f;
	}
	else
	{
		_LerpRate = 0.0f;
	}
	
	//カメラの注視点の線形補間を行う(プレイヤーから歴史書に向けて補間)。
	_LerpCameraLookAtPos = (_HistoryBookPos * (1.0f - _LerpRate) + ((*_PlayerPos)) * _LerpRate);

	//カメラの位置の線形補間を行う(ゲームカメラの位置からプレイヤーの位置に向けて補間)。
	_LerpCameraPos = (((*_PlayerPos) + PLAYER_HEIGHT) * (1.0f - _LerpRate) + (_Player->transform->GetPosition()+Vector3(0.0f,10.0f,0.0f)) * _LerpRate);

	//カメラの注視点を線形補間された位置に設定。
	transform->LockAt((_LerpCameraLookAtPos));
	_Camera->SetTarget(_LerpCameraLookAtPos);

	//カメラの位置を線形補完された位置に設定。。
	transform->SetPosition((_LerpCameraPos));
}
