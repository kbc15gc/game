#pragma once

namespace Event
{
	enum class EventID:int
	{
		None = -1,
		ShopF,
	};
}

class Player;
class PlayerCamera;

//イベントを管理するクラス
class EventManager
{
public:
	EventManager();
	//添え字に対応したイベントを実行。
	//[in] 実行するイベントのID
	//[in] 渡す引数。
	bool Execute(Event::EventID id,unsigned int idx = -1);

	//イベントの終了通知を送る。
	void NotifyEndEvent();

	static EventManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new EventManager();
			//イベント追加。
			_Instance->AddEvent();
		}
		return _Instance;
	}
private:
	Player* GetPlayer()
	{
		if (_Player == nullptr)
		{
			_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
		}
		return _Player;
	}

	PlayerCamera* GetCamera()
	{
		if (!_Camera)
		{
			_Camera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
		}
		return _Camera;
	}

	//最初にイベントを追加する処理。
	void AddEvent();
	//イベント開始。
	void StartEvent();
private:
	static EventManager* _Instance;

	//関数を保持するリスト。
	typedef std::function<void (int)> EventFunc;
	typedef std::function<void(void)> EventAction;
	vector<EventFunc> _FuncList;
	vector<EventAction> _ActionList;

	//現在実行中のイベントの添え字。
	Event::EventID _ActiveEvent = Event::EventID::None;

	Player* _Player;
	PlayerCamera* _Camera;
};