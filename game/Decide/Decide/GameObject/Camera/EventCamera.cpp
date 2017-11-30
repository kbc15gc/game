#include "stdafx.h"
#include "EventCamera.h"

void EventCamera::Awake()
{
	GameCamera::Awake();

	//カメラコンポーネント
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
	//実行フラグ設定。
	_Runtime = true;
	//タイマー初期化。
	_Timer = 0.0f;
	//インデックス初期化。
	_Index = 0;

	//このカメラを使う。
	ActiveCamera();

	//CSVから情報取得。
	vector<unique_ptr<EventCameraInfo>> list;
	Support::LoadCSVData<EventCameraInfo>("Asset/Data/EventCameraInfo.csv", EventCameraData, ARRAY_SIZE(EventCameraData), list);
	_Info = *list[id].get();
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
	}
	else
	{
		//次があるか？
		if (_Index + 1< _Info.arraynum)
		{
			//次に移る前に、現在の目標時間分を引く。
			_Timer -= _Info.times[_Index++];
		}
		else
		{
			//終了。
			_Runtime = false;
			//次のカメラに変更(PlayerCamera)。
			if (_NextCamera) {
				// 次のカメラを有効化。
				_NextCamera->ActiveCamera();
			}
		}
	}
}