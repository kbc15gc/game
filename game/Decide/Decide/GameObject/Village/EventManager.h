#pragma once

namespace Event
{
	enum class EventID:int
	{
		Shop,
	};
}

//�C�x���g���Ǘ�����N���X
class EventManager
{
public:
	EventManager();
	//�Y�����ɑΉ������C�x���g�����s�B
	bool Execute(Event::EventID event,int idx);

	static EventManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new EventManager();
			_Instance->AddEvent();
		}
		return _Instance;
	}
private:
	void AddEvent();
private:
	typedef std::function<void (int)> EventFunc;
	vector<EventFunc> _EventList;
	static EventManager* _Instance;
};