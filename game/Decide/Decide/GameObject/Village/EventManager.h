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

//�C�x���g���Ǘ�����N���X
class EventManager
{
public:
	EventManager();
	//�Y�����ɑΉ������C�x���g�����s�B
	//[in] ���s����C�x���g��ID
	//[in] �n�������B
	bool Execute(Event::EventID id,unsigned int idx = -1);

	//�C�x���g�̏I���ʒm�𑗂�B
	void NotifyEndEvent();

	static EventManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new EventManager();
			//�C�x���g�ǉ��B
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

	//�ŏ��ɃC�x���g��ǉ����鏈���B
	void AddEvent();
	//�C�x���g�J�n�B
	void StartEvent();
private:
	static EventManager* _Instance;

	//�֐���ێ����郊�X�g�B
	typedef std::function<void (int)> EventFunc;
	typedef std::function<void(void)> EventAction;
	vector<EventFunc> _FuncList;
	vector<EventAction> _ActionList;

	//���ݎ��s���̃C�x���g�̓Y�����B
	Event::EventID _ActiveEvent = Event::EventID::None;

	Player* _Player;
	PlayerCamera* _Camera;
};