#include "stdafx.h"
#include "EventManager.h"
#include "GameObject\Village\Shop\Shop.h"

EventManager*  EventManager::_Instance = nullptr;

EventManager::EventManager()
{
}

bool EventManager::Execute(Event::EventID event, int idx)
{
	EventFunc func;
	//範囲外アクセスをチェック。
	try {
		func = _EventList.at((int)event);
	}
	catch (const out_of_range& oor) {
		oor.what();
		return false;
	}
	func(idx);
	return true;	
}

void EventManager::AddEvent()
{
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("Shop", 0);
	//関数を追加。

	//ショップを開く処理。
	_EventList.push_back(std::bind(&Shop::OpenShop, shop, std::placeholders::_1));
}