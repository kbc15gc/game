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
	//範囲外アクセスをチェック。
	try 
	{
		//関数かアクションか？
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
	//関数を追加。

	//ショップを開く処理。
	_FuncList.push_back(std::bind(&Shop::OpenShop, shop, std::placeholders::_1));
}

void EventManager::StartEvent()
{
}