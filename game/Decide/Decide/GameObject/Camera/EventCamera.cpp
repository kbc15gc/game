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
	if(_Runtime)
	{
		//�t�F�[�h���邩�H
		if (_Info.fade[_Index])
		{
			if (Scene::GetFadeState() == fbScene::FadeStateE::EndFadeOut)
				_Move();
			else if (Scene::GetFadeState() == fbScene::FadeStateE::EndFadeIn)
			{
				//�C�x���g�J�����ɐ؂�ւ���B
				ActiveCamera();
				Scene::StartFade(false, FADE_TIME);
			}
		}else
		{
			_Move();
		}
	}
	else
	{	
		if (_Info.fade[_Index])
		{
			//�C�x���g�J�����I���B
			if (Scene::GetFadeState() == fbScene::FadeStateE::EndFadeIn)
			{
				EndEvent();
				Scene::StartFade(false, FADE_TIME);
			}
		}else
		{
			EndEvent();
		}
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
	transform->SetPosition(_Info.pos[_Index]);
	transform->SetRotation(_Info.rot[_Index]);

	if (_Info.fade[_Index])
	{
		//�t�F�[�h�J�n�B
		Scene::StartFade(true, FADE_TIME);
	}else
	{
		//�C�x���g�J�����ɐ؂�ւ���B
		ActiveCamera();
	}
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
	if(_Timer <= _Info.times[_Index])
	{
		//�ړ�
		auto pos = Vector3::Lerp(_Info.pos[_Index], _Info.pos[_Index + 1], _Timer);
		transform->SetPosition(pos);
		auto rot = Quaternion::Lerp(_Info.rot[_Index], _Info.rot[_Index + 1], _Timer);
		transform->SetRotation(rot);
	}
	else
	{
		//�������邩�H
		if (++_Index < _Info.size-1)
		{
			//��O�̖ڕW���ԕ��������B
			_Timer -= _Info.times[_Index - 1];
		}
		else
		{
			//�I���B
			_Runtime = false;
		}
		if (_Info.fade[_Index])
		{
			//�t�F�[�h�J�n�B
			Scene::StartFade(true, FADE_TIME);
		}
	}
}