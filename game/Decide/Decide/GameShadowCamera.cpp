#include"stdafx.h"
#include "GameShadowCamera.h"
#include "fbEngine\_Object\_Component\_3D\ShadowCamera.h"
#include "GameObject/Player/Player.h"

void GameShadowCamera::Start()
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	

}

void GameShadowCamera::Update()
{
	ShadowMap* shadow = INSTANCE(SceneManager)->GetShadowMap();
	
	Vector3 lightPos;

	lightPos.Add(_Player->transform->GetPosition(), Vector3(1, 1, 0));
	shadow->SetLightPosition(lightPos);
	shadow->SetLightTarget(_Player->transform->GetPosition());
	shadow->SetNear(1.0f);
	shadow->SetFar(100.0f);

	/*if (KeyBoardInput->isPressed(DIK_U))
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
	}*/
	/*Vector3 pos = _Player->transform->GetPosition();
	camera->SetViewPoint(pos);
	transform->SetPosition(pos + _Dist);*/
}