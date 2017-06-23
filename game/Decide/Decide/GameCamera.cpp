#include "stdafx.h"
#include "GameCamera.h"

GameCamera::~GameCamera()
{

}

void GameCamera::Awake()
{

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
			// ŽŸ‚ÌƒJƒƒ‰‚ð—LŒø‰»B
			_NextCamera->ActiveCamera();
		}
	}
}

void GameCamera::SetNextCamera(GameCamera* next)
{
	_NextCamera = next;
}

void GameCamera::Move()
{
	
}