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

	//カメラコンポーネント
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
		//フェードするか？
		if (_Info.fade[_Index])
		{
			if (Scene::GetFadeState() == fbScene::FadeStateE::EndFadeOut)
				_Move();
			else if (Scene::GetFadeState() == fbScene::FadeStateE::EndFadeIn)
			{
				//イベントカメラに切り替える。
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
			//イベントカメラ終了。
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
	//実行フラグ設定。
	_Runtime = true;
	//タイマー初期化。
	_Timer = 0.0f;
	//インデックス初期化。
	_Index = 0;

	_isActive = true;

	//CSVから情報取得。
	vector<unique_ptr<EventCameraInfo>> list;
	Support::LoadCSVData<EventCameraInfo>("Asset/Data/EventCameraInfo.csv", EventCameraData, ARRAY_SIZE(EventCameraData), list);
	_Info = *list[id].get();
	transform->SetPosition(_Info.pos[_Index]);
	transform->SetRotation(_Info.rot[_Index]);

	if (_Info.fade[_Index])
	{
		//フェード開始。
		Scene::StartFade(true, FADE_TIME);
	}else
	{
		//イベントカメラに切り替える。
		ActiveCamera();
	}
}

void EventCamera::EndEvent()
{
	//次のカメラに変更(PlayerCamera)。
	if (_NextCamera) {
		// 次のカメラを有効化。
		_NextCamera->ActiveCamera();
	}
	//終了を通知。
	INSTANCE(EventManager)->NotifyEndEvent();
}

void EventCamera::_Move()
{
	//時間加算。
	_Timer += Time::DeltaTime();
	if(_Timer <= _Info.times[_Index])
	{
		//移動
		auto pos = Vector3::Lerp(_Info.pos[_Index], _Info.pos[_Index + 1], _Timer);
		transform->SetPosition(pos);
		auto rot = Quaternion::Lerp(_Info.rot[_Index], _Info.rot[_Index + 1], _Timer);
		transform->SetRotation(rot);
	}
	else
	{
		//次があるか？
		if (++_Index < _Info.size-1)
		{
			//一つ前の目標時間分を引く。
			_Timer -= _Info.times[_Index - 1];
		}
		else
		{
			//終了。
			_Runtime = false;
		}
		if (_Info.fade[_Index])
		{
			//フェード開始。
			Scene::StartFade(true, FADE_TIME);
		}
	}
}