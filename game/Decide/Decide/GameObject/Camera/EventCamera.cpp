#include "stdafx.h"
#include "EventCamera.h"
#include "GameObject\Village\EventManager.h"
#include "fbEngine\_Scene\Scene.h"

namespace {
	static const int FADE_TIME = 1.0f;
}

void EventCamera::Awake()
{
	GameCamera::Awake();

	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.1f);
	_Camera->SetFar(1500.0f);
	_Camera->SetUseTarget(false);
	
	UnActivateFlg();
}

void EventCamera::Start()
{

}

void EventCamera::UpdateSubClass()
{
	if (_Runtime)
	{
		_Move();
	}
	else
	{
		auto time = _Info.time[_Index];
		_Timer += Time::DeltaTime();
		//�����v�Z�B
		auto rate = (0.0f < time) ? _Timer / time : 1.0f;
		//�ړ��B
		auto pos = Vector3::Lerp(_NowPos, _BeforeCamera->GetPosition(), rate);
		//��]�B
		auto rot = Quaternion::Lerp(_NowRot, _BeforeCamera->GetRotation(), rate);
		transform->SetPosition(pos);
		transform->SetRotation(rot);

		if(_Timer >= time)
		{
			EndEvent();
		}
	}

	if(Scene::GetFadeState() == fbScene::FadeStateE::EndFadeIn)
	{
		//�Œ�l(��)
		Scene::StartFade(false, FADE_TIME);
	}
}

void EventCamera::Excute(int id)
{
	//���s�t���O�ݒ�B
	_Runtime = true;
	//�^�C�}�[�������B
	_Timer = 0.0f;
	//�C���f�b�N�X�������B
	_Index = 0;

	_isActive = true;

	//CSV������擾�B
	vector<unique_ptr<EventCameraInfo>> list;
	Support::LoadCSVData<EventCameraInfo>("Asset/Data/EventCameraInfo.csv", EventCameraData, ARRAY_SIZE(EventCameraData), list);
	_Info = *list[id].get();
	_BeforeCamera = INSTANCE(GameObjectManager)->mainCamera->transform;
	_NowPos = _BeforeCamera->GetPosition();
	_NowRot = _BeforeCamera->GetRotation();

	transform->SetPosition(_NowPos);
	transform->SetRotation(_NowRot);
	_Camera->Update();


	//�C�x���g�J�����ɐ؂�ւ���B
	ActiveCamera();
}

void EventCamera::EndEvent()
{
	//���̃J�����ɕύX(PlayerCamera)�B
	if (_NextCamera) {
		// ���̃J������L�����B
		_NextCamera->ActiveCamera();
	}
	//�I����ʒm�B
	INSTANCE(EventManager)->NotifyEndEvent();
}

void EventCamera::_Move()
{
	//���ԉ��Z�B
	_Timer += Time::DeltaTime();
	auto time = _Info.time[_Index];
	//�⊮�Ƃ��B
	//�����v�Z�B
	auto rate = (0.0f < time) ? _Timer / time : 1.0f;
	//�ړ��B
	auto pos = Vector3::Lerp(_NowPos, _Info.pos[_Index], rate);
	//��]�B
	auto rot = Quaternion::Lerp(_NowRot, _Info.rot[_Index], rate);
	transform->SetPosition(pos);
	transform->SetRotation(rot);

	if(_Timer <= time)
	{
		auto fade = _Info.fade[_Index];
		//�t�F�[�h���邩�H
		if (fade >= 0.0f &&
			_Timer >= fade)
		{
			Scene::StartFade(true, time - fade);
		}
	}
	else
	{
		_NowPos = _Info.pos[_Index];
		_NowRot = _Info.rot[_Index];
		//�������邩�H
		if (_Index < _Info.size - 2)
		{
			//��O�̖ڕW���ԕ��������B
			_Timer -= time;
		}
		else
		{
			//�I���B
			_Runtime = false;
			_Timer = 0.0f;
		}
		_Index++;
	}
}