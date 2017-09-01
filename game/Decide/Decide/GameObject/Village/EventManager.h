#pragma once

namespace Event
{
	enum class EventID:int
	{
		Shop,
	};
}

//イベントを管理するクラス
class EventManager
{
public:
	EventManager();
	//添え字に対応したイベントを実行。
	bool Execute(Event::EventID event,int idx);

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
	//最初にイベントを追加する処理。
	void AddEvent();
private:
	typedef std::function<void (int)> EventFunc;
	vector<EventFunc> _EventList;
	static EventManager* _Instance;
};