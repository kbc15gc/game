#include "stdafx.h"
#include "GameCamera.h"

GameCamera::~GameCamera()
{

}

void GameCamera::Awake()
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

void GameCamera::Start()
{

}

void GameCamera::Update()
{
	if (_isActive) {
		UpdateSubClass();
		ChangeRequest();
	}
}

void GameCamera::ChangeRequest()
{
	if ((KeyBoardInput->isPush(DIK_C)))
	{
		if (_NextCamera) {
			// 次のカメラを有効化。
			_NextCamera->ActiveCamera();
		}
	}
}

void GameCamera::SetNextCamera(GameCamera* next)
{
	_NextCamera = next;
}
