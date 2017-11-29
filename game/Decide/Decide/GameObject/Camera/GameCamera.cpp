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
	if ((KeyBoardInput->isPush(DIK_C)))
	{
		if (_NextCamera) {
			// 次のカメラを有効化。
			_NextCamera->ActiveCamera();
		}
	}
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

Vector3 GameCamera::_SpringChaseMove(const Vector3 & now, const Vector3 & target, float spring, float damping, float time, float speed)
{
	//ワールド行列でのカメラの理想位置。
	//auto vIdealPos = _DestinationPos;

	//この理想位置へのバネによる加速度を計算。
	auto vDisplace = now - target;
	auto vSpringAccel = (-spring * vDisplace) - (damping * _Velocity);
	//オイラー積分を使ってカメラの速度と位置を更新。
	_Velocity += vSpringAccel * time * speed;
	auto next = now + (_Velocity * time);
	auto diff = next - target;

	return next;
}