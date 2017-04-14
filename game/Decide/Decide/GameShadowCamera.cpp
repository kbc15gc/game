#include "GameShadowCamera.h"
#include "fbEngine/ShadowCamera.h"
#include "GameObject/Player/Player.h"

void GameShadowCamera::Awake()
{
	ShadowCamera* camera = AddComponent<ShadowCamera>();
	GameObjectManager::mainShadowCamera = camera;

	_Player = (Player*)GameObjectManager::FindObject("Player");
	transform->SetLocalAngle(Vector3(-90.0f, 0.0f, 0.0f));
	transform->SetLocalPosition(_Player->transform->LocalPos(Vector3(0.0f, 5.0f, 0.0f)));
	camera->SetNear(1.0f);
	camera->SetFar(1000.0f);
}

void GameShadowCamera::Update()
{
	//í≤êÆóp
	ShadowCamera* camera = GetComponent<ShadowCamera>();
	/*static Vector2 nf = { 2544, 4310 };
	if (KeyBoardInput->isPressed(DIK_N))
	{
		nf.x++;
	}
	if (KeyBoardInput->isPressed(DIK_M))
	{
		nf.x--;
	}
	if (KeyBoardInput->isPressed(DIK_F))
	{
		nf.y++;
	}
	if (KeyBoardInput->isPressed(DIK_G))
	{
		nf.y--;
	}*/

	/*camera->Near(nf.x);
	camera->Far(nf.y);*/

	Vector3 angle = transform->GetAngle();

	if (KeyBoardInput->isPressed(DIK_LEFT))
	{
		angle.y -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_RIGHT))
	{
		angle.y += 1;
	}
	if (KeyBoardInput->isPressed(DIK_UP))
	{
		angle.x -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_DOWN))
	{
		angle.x += 1;
	}
	transform->SetAngle(angle);

	Vector3 dir = Vector3::zero;
	if (KeyBoardInput->isPressed(DIK_U))
	{
		dir.z += 1;
	}
	if (KeyBoardInput->isPressed(DIK_I))
	{
		dir.z -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_O))
	{
		dir.x -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_P))
	{
		dir.x += 1;
	}
	if (KeyBoardInput->isPressed(DIK_K))
	{
		dir.y += 1;
	}
	if (KeyBoardInput->isPressed(DIK_L))
	{
		dir.y -= 1;
	}
	camera->SetViewPoint(_Player->transform->GetLocalPosition());
	//transform->SetLocalPosition(_Player->transform->LocalPos(Vector3(0.0f, 500.0f, 0.0f)));

	//transform->localPosition.Add(transform->Direction(dir));
	Vector3 pos = transform->GetLocalPosition();
	transform->SetLocalPosition(pos + dir);
}