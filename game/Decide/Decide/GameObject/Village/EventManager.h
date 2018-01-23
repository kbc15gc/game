#pragma once

namespace Event
{
	enum class EventID :int
	{
		None = -1,
		ShopF = 0,
		EventCameraF,
		FunctionNum,

		StatusWindowA = FunctionNum,
		HistoryBookA,
		ActionNum
	};
}

class Player;
class PlayerCamera;

//�C�x���g���Ǘ�����N���X
class EventManager
{
public:
	EventManager();
	//������
	void ReSet();
	//�Y�����ɑΉ������C�x���g�����s�B
	//[in] ���s����C�x���g��ID
	//[in] �n�������B
	bool Execute(Event::EventID id,int idx = -1);

	//�C�x���g�̏I���ʒm�𑗂�B
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
	//���݃C�x���g�Đ������ǂ����H
	bool IsEvent()
	{
		return (_ActiveEvent != Event::EventID::None);
	}
	//���݂̃C�x���g��ԋp�B
	Event::EventID GetEventID()
	{
		return _ActiveEvent;
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
		if (!_Pcamera)
		{
			_Pcamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
		}
		return _Pcamera;
	}
	//�ŏ��ɃC�x���g��ǉ����鏈���B
	void AddEvent();
private:
	static EventManager* _Instance;

	//�֐���ێ����郊�X�g�B
	typedef std::function<void (int)> EventFunc;
	typedef std::function<void(void)> EventAction;
	vector<EventFunc> _FuncList;
	vector<EventAction> _ActionList;

	//���ݎ��s���̃C�x���g�̓Y�����B
	Event::EventID _ActiveEvent = Event::EventID::None;
private:
	Player* _Player;
	PlayerCamera* _Pcamera;
};