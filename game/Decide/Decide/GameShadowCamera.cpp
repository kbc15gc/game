#include "GameShadowCamera.h"
#include "fbEngine\_Object\_Component\_3D\ShadowCamera.h"
#include "GameObject/Player/Player.h"

void GameShadowCamera::Awake()
{
	ShadowCamera* camera = AddComponent<ShadowCamera>();
	INSTANCE(GameObjectManager)->mainShadowCamera = camera;
	
	camera->SetNear(1.0f);
	camera->SetFar(50.0f);

	camera->SetViewVolume(Vector2(20, 20));
}

void GameShadowCamera::Start()
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	_Dist = Vector3(0.0f, 10.0f, -10.0f);
}

void GameShadowCamera::Update()
{
	//í≤êÆóp
	ShadowCamera* camera = GetComponent<ShadowCamera>();

	if (KeyBoardInput->isPressed(DIK_U))
	{
		_Dist.z += 1;
	}
	if (KeyBoardInput->isPressed(DIK_I))
	{
		_Dist.z -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_O))
	{
		_Dist.x -= 1;
	}
	if (KeyBoardInput->isPressed(DIK_P))
	{
		_Dist.x += 1;
	}
	if (KeyBoardInput->isPressed(DIK_K))
	{
		_Dist.y += 1;
	}
	if (KeyBoardInput->isPressed(DIK_L))
	{
		_Dist.y -= 1;
	}
	Vector3 pos = _Player->transform->GetPosition();
	camera->SetViewPoint(pos);
	transform->SetPosition(pos + _Dist);
}