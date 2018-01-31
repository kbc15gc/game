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

void GameCamera::LateUpdate()
{
	if (_isActivate) {
		_isActive = true;
		_isActivate = false;
	}
}

void GameCamera::ChangeRequest()
{
#ifdef _Debug
	if ((KeyBoardInput->isPush(DIK_C)))
	{
		if (_NextCamera) {
			// ���̃J������L�����B
			_NextCamera->ActiveCamera();
		}
	}
#endif
}

void GameCamera::ActiveCamera()
{
	Camera* cam = INSTANCE(GameObjectManager)->mainCamera;	//GameObjectManager�ɓo�^���Ă���ꍇ�A���O���čX�V���~����B
	if (cam) {
		static_cast<GameCamera*>(cam->gameObject)->UnActivateFlg();
	}
	INSTANCE(GameObjectManager)->mainCamera = this->_Camera;
	//�J�������ς�������̃A�N�V�����B
	ChangeCameraReAction();
	_isActivate = true;
}
