#include "stdafx.h"
#include "EventCamera.h"

void EventCamera::Awake()
{
	GameCamera::Awake();

	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.1f);
	_Camera->SetFar(1500.0f);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;
}

void EventCamera::Start()
{
	UnActivateFlg();
}

void EventCamera::UpdateSubClass()
{
	if(_Runtime)
	{
		_Move();
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

	//���̃J�������g���B
	ActiveCamera();

	//CSV������擾�B
	vector<unique_ptr<EventCameraInfo>> list;
	Support::LoadCSVData<EventCameraInfo>("Asset/Data/EventCameraInfo.csv", EventCameraData, ARRAY_SIZE(EventCameraData), list);
	_Info = *list[id].get();
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
	}
	else
	{
		//�������邩�H
		if (_Index + 1< _Info.arraynum)
		{
			//���Ɉڂ�O�ɁA���݂̖ڕW���ԕ��������B
			_Timer -= _Info.times[_Index++];
		}
		else
		{
			//�I���B
			_Runtime = false;
			//���̃J�����ɕύX(PlayerCamera)�B
			if (_NextCamera) {
				// ���̃J������L�����B
				_NextCamera->ActiveCamera();
			}
		}
	}
}