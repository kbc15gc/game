#pragma once

namespace Event
{
	enum class EventID :int
	{
		None = -1,
		ShopF = 0,
		FunctionNum,

		StatusWindowA = FunctionNum,
		HistoryBookA,
		ActionNum
	};
}

class Player;
class PlayerCamera;

//イベントを管理するクラス
class EventManager
{
public:
	EventManager();
	//初期化
	void ReSet();
	//添え字に対応したイベントを実行。
	//[in] 実行するイベントのID
	//[in] 渡す引数。
	bool Execute(Event::EventID id,int idx = -1);

	//イベントの終了通知を送る。
	void NotifyEndEvent();

	static EventManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new EventManager();
		}
		return _Instance;
	}

	bool ShopEvent()
	{
		if (_ActiveEvent == Event::EventID::ShopF)
		{
			return true;
		}
		return false;
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
private:
	static EventManager* _Instance;

	//関数を保持するリスト。
	typedef std::function<void (int)> EventFunc;
	typedef std::function<void(void)> EventAction;
	vector<EventFunc> _FuncList;
	vector<EventAction> _ActionList;

	//現在実行中のイベントの添え字。
	Event::EventID _ActiveEvent = Event::EventID::None;
private:
	Player* _Player;
	PlayerCamera* _Camera;
};