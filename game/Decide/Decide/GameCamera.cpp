#include "GameCamera.h"
#include "fbEngine/Camera.h"
#include "GameObject\Player\Player.h"

namespace
{
	const Vector3 PLAYER_HEIGHT(0.0f, 1.5f, 0.0f);
	const float CAMERA_SPEED = 1.0f;
}

void GameCamera::Awake()
{
	Camera* camera = AddComponent<Camera>();
	GameObjectManager::mainCamera = camera;
	camera->SetNear(1);
	camera->SetFar(1000);
	//プレイヤーを検索
	_Player = (Player*)GameObjectManager::FindObject("Player");
	//プレイヤーの少し後ろにカメラのポジションをセット。
	transform->SetLocalPosition(Vector3(0, 0, -10));
	_ToPos = D3DXVECTOR3(0.0f, 2.0f, -3.0f);
}

void GameCamera::Update()
{
	//調整用
	Camera* camera = GetComponent<Camera>();
	//static Vector2 nf = { 1, 1000 };
	//if (KeyBoardInput->isPressed(DIK_N))
	//{
	//	nf.x++;
	//}
	//if (KeyBoardInput->isPressed(DIK_M))
	//{
	//	nf.x--;
	//}
	//if (KeyBoardInput->isPressed(DIK_F))
	//{
	//	nf.y++;
	//}
	//if (KeyBoardInput->isPressed(DIK_G))
	//{
	//	nf.y--;
	//}

	//camera->Near(nf.x);
	//camera->Far(nf.y);
	

	/*if (KeyBoardInput->isPressed(DIK_LEFT))
	{
	transform->localAngle.y -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_RIGHT))
	{
	transform->localAngle.y += 1;
	}
	if (KeyBoardInput->isPressed(DIK_UP))
	{
	transform->localAngle.x -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_DOWN))
	{
	transform->localAngle.x += 1;
	}*/

	Vector3 dir = Vector3::zero;
	/*if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z += 1;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x += 1;
	}
	if (KeyBoardInput->isPressed(DIK_O))
	{
		dir.y += 1;
	}
	if (KeyBoardInput->isPressed(DIK_L))
	{
		dir.y -= 1;
	}*/
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
	
	transform->SetLocalPosition(_Player->transform->GetLocalPosition() + (Vector3)_ToPos);
	//プレイヤーの高さ分を足す
	camera->SetViewPoint(_Player->transform->GetLocalPosition() + PLAYER_HEIGHT);
	//transform->localPosition.Add(transform->Direction(dir));
	//transform->LockAt(player);
}

void GameCamera::LateUpdate()
{
	
}

void GameCamera::RotTransversal(float roty)
{
	D3DXQUATERNION mAxisY;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	D3DXQuaternionRotationAxis(&mAxisY, &(const D3DXVECTOR3&)Vector3::up, roty);
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);
	D3DXVec3Transform(&v, &_ToPos, &rot);
	_ToPos.x = v.x;
	_ToPos.y = v.y;
	_ToPos.z = v.z;
}

void GameCamera::RotLongitudinal(float rotx)
{
	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &_ToPos);
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	D3DXVec3Transform(&v, &_ToPos, &rot);
	D3DXVECTOR3 toPosOld = _ToPos;
	_ToPos.x = v.x;
	_ToPos.y = v.y;
	_ToPos.z = v.z;
	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &_ToPos);
	//カメラの上下の上限
	if (toPosDir.y < -0.5f)
	{
		_ToPos = toPosOld;
	}
	else if (toPosDir.y > 0.8f)
	{
		_ToPos = toPosOld;
	}

}