#include "stdafx.h"
#include "EventManager.h"

#include "GameObject\Player\Player.h"
#include "GameObject\Camera\PlayerCamera.h"

#include "GameObject\Village\Shop\Shop.h"
#include "GameObject\StatusWindow\StatusWindow.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"

EventManager*  EventManager::_Instance = nullptr;

EventManager::EventManager()
{
}

bool EventManager::Execute(Event::EventID id, int idx)
{
	//他のイベントが実行中ならやめる。
	if (_ActiveEvent != Event::EventID::None && _ActiveEvent != id)
		return false;

	//範囲外アクセスをチェック。
	try 
	{
		//止める。
		GetPlayer()->PlayerStopEnable();
		GetCamera()->SetIsMove(false);
		//実行中のイベントの添え字保持。
		_ActiveEvent = id;

		//関数かアクションか？
		if (idx >= 0)
			_FuncList.at((int)id)(idx);
		else
			_ActionList.at((int)id - (int)Event::EventID::FunctionNum)();
	}
	//範囲外アクセス。
	catch (const out_of_range& oor) 
	{
		oor.what();
		NotifyEndEvent();
		return false;
	}

	return true;
}

void EventManager::NotifyEndEvent()
{
	GetPlayer()->PlayerStopDisable();
	GetCamera()->SetIsMove(true);
	_ActiveEvent = Event::EventID::None;
}

void EventManager::AddEvent()
{
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("Shop", 0);
	
	StatusWindow* status = (StatusWindow*)INSTANCE(GameObjectManager)->FindObject("StatusWindow");
	HistoryBook* book = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");
	
	//関数を追加。

	//ショップを開く処理。
	_FuncList.push_back(std::bind(&Shop::OpenShop, shop, std::placeholders::_1));

	//

	//ステータスウィンドウの開閉処理。
	_ActionList.push_back([status]
	{
		status->SetActive(!status->GetActive(), true);
	});
	//歴史書の開閉処理。
	_ActionList.push_back([book]
	{
		book->SetActive(!book->GetActive(), true);
	});
}

void EventManager::StartEvent()
{
}