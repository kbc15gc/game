#include "GameCamera.h"
#include "fbEngine/Camera.h"
#include "Player.h"

void GameCamera::Awake()
{
	Camera* camera = AddComponent<Camera>();
	GameObjectManager::mainCamera = camera;
	//transform->localPosition = Vector3(200, 500, 500);
	transform->SetLocalPosition(Vector3(0, 0, -10));
	//transform->localAngle = Vector3(40, 0, 0);
	camera->SetNear(1);
	camera->SetFar(1000);
	_ToPos = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
}

void GameCamera::Update()
{
	//í≤êÆóp
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
	if (KeyBoardInput->isPressed(DIK_RIGHT))
	{
		RotTransversal(-0.1f);
		
	}
	if (KeyBoardInput->isPressed(DIK_LEFT))
	{
		RotTransversal(0.1f);
		
	}
	
	Player* player = (Player*)GameObjectManager::FindObject("Player");
	transform->SetLocalPosition(player->transform->GetLocalPosition() + (Vector3)_ToPos);
	camera->SetViewPoint(player->transform->GetLocalPosition());
	dir = (Vector3)_ToPos;
	dir.Normalize();
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
	D3DXQuaternionRotationAxis(&mAxisY, &D3DXVECTOR3{0.0f,1.0f,0.0f}, roty);
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);
	D3DXVec3Transform(&v, &_ToPos, &rot);
	_ToPos.x = v.x;
	_ToPos.y = v.y;
	_ToPos.z = v.z;
}