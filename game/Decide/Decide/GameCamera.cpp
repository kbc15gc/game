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
	_Camera->SetNear(1);
	_Camera->SetFar(1000);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;

	//カメラのコリジョンの半径設定
	_Radius = 0.5f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//距離の下限と上限
	_Dist = 7.0f;
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
}

void GameCamera::Update()
{
	if (true)
	{
		_StandardBehavior();
	}
	else
	{
		_HistoryBehavior();
	}
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
	fbPhysicsCallback::ClosestConvexResultCallback ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere,from, to);
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
	//プレイヤーの方を向く
	transform->LockAt((*_PlayerPos) + PLAYER_HEIGHT);
}

void GameCamera::_HistoryBehavior()
{

	//カメラの位置をプレイヤーの顔ぐらいに設定。
	transform->SetLocalPosition((*_PlayerPos) + PLAYER_HEIGHT);

	_HistoryBookPos = &_HistoryBook->transform->GetPosition();

	//カメラの注視点をHistoryBookのモデルに設定。
	transform->LockAt((*_HistoryBookPos));
}
