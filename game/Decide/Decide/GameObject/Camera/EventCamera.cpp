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
	if (_Runtime)
	{
		_Move();
	}
	else
	{
		auto time = _Info.time[_Index];
		_Timer += Time::DeltaTime();
		//割合計算。
		auto rate = (0.0f < time) ? _Timer / time : 1.0f;
		//移動。
		auto pos = Vector3::Lerp(_NowPos, _BeforeCamera->GetPosition(), rate);
		//回転。
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
		//固定値(仮)
		Scene::StartFade(false, FADE_TIME);
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
	_BeforeCamera = INSTANCE(GameObjectManager)->mainCamera->transform;
	_NowPos = _BeforeCamera->GetPosition();
	_NowRot = _BeforeCamera->GetRotation();

	transform->SetPosition(_NowPos);
	transform->SetRotation(_NowRot);
	_Camera->Update();


	//イベントカメラに切り替える。
	ActiveCamera();
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
	auto time = _Info.time[_Index];
	//補完とか。
	//割合計算。
	auto rate = (0.0f < time) ? _Timer / time : 1.0f;
	//移動。
	auto pos = Vector3::Lerp(_NowPos, _Info.pos[_Index], rate);
	//回転。
	auto rot = Quaternion::Lerp(_NowRot, _Info.rot[_Index], rate);
	transform->SetPosition(pos);
	transform->SetRotation(rot);

	if(_Timer <= time)
	{
		auto fade = _Info.fade[_Index];
		//フェードするか？
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
		//次があるか？
		if (_Index < _Info.size - 2)
		{
			//一つ前の目標時間分を引く。
			_Timer -= time;
		}
		else
		{
			//終了。
			_Runtime = false;
			_Timer = 0.0f;
		}
		_Index++;
	}
}