#include "stdafx.h"
#include "EventManager.h"

#include "GameObject\Player\Player.h"
#include "GameObject\Camera\PlayerCamera.h"

#include "GameObject\Village\Shop\Shop.h"

EventManager*  EventManager::_Instance = nullptr;

EventManager::EventManager()
{
}

bool EventManager::Execute(Event::EventID id, unsigned int idx)
{
	//�͈͊O�A�N�Z�X���`�F�b�N�B
	try 
	{
		//�֐����A�N�V�������H
		if (idx >= 0)
			_FuncList.at((int)id)(idx);
		else
			_ActionList.at((int)id)();

		GetPlayer()->PlayerStopEnable();
		GetCamera()->SetIsMove(false);
	}
	catch (const out_of_range& oor) 
	{
		oor.what();
		return false;
	}
	_ActiveEvent = id;

	return true;
}

void EventManager::NotifyEndEvent()
{
	GetPlayer()->PlayerStopDisable();
	GetCamera()->SetIsMove(true);
}

void EventManager::AddEvent()
{
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("Shop", 0);
	//�֐���ǉ��B

	//�V���b�v���J�������B
	_FuncList.push_back(std::bind(&Shop::OpenShop, shop, std::placeholders::_1));
}

void EventManager::StartEvent()
{
}