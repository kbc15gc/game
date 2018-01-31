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
			// 次のカメラを有効化。
			_NextCamera->ActiveCamera();
		}
	}
#endif
}

void GameCamera::ActiveCamera()
{
	Camera* cam = INSTANCE(GameObjectManager)->mainCamera;	//GameObjectManagerに登録している場合、取り外して更新を停止する。
	if (cam) {
		static_cast<GameCamera*>(cam->gameObject)->UnActivateFlg();
	}
	INSTANCE(GameObjectManager)->mainCamera = this->_Camera;
	//カメラが変わった時のアクション。
	ChangeCameraReAction();
	_isActivate = true;
}
