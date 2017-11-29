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
			// ���̃J������L�����B
			_NextCamera->ActiveCamera();
		}
	}
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

Vector3 GameCamera::_SpringChaseMove(const Vector3 & now, const Vector3 & target, float spring, float damping, float time, float speed)
{
	//���[���h�s��ł̃J�����̗��z�ʒu�B
	//auto vIdealPos = _DestinationPos;

	//���̗��z�ʒu�ւ̃o�l�ɂ������x���v�Z�B
	auto vDisplace = now - target;
	auto vSpringAccel = (-spring * vDisplace) - (damping * _Velocity);
	//�I�C���[�ϕ����g���ăJ�����̑��x�ƈʒu���X�V�B
	_Velocity += vSpringAccel * time * speed;
	auto next = now + (_Velocity * time);
	auto diff = next - target;

	return next;
}