#include "stdafx.h"
#include "EventManager.h"

#include "GameObject\Player\Player.h"
#include "GameObject\Camera\PlayerCamera.h"
#include "GameObject\Camera\EventCamera.h"

#include "GameObject\Village\Shop\Shop.h"
#include "GameObject\StatusWindow\StatusWindow.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"

#include "GameObject\WorldMap\WorldMap.h"

EventManager*  EventManager::_Instance = nullptr;

EventManager::EventManager()
{
}

void EventManager::ReSet()
{
	_FuncList.clear();
	_ActionList.clear();
	_ActiveEvent = Event::EventID::None;

	_Player = nullptr;
	_Pcamera = nullptr;
}

bool EventManager::Execute(Event::EventID id, int idx)
{
	//�C�x���g���Ȃ���I
	if(_ActionList.size() <= 0)
		//�C�x���g�ǉ��B
		_Instance->AddEvent();

	//���̃C�x���g�����s���Ȃ��߂�B
	if (_ActiveEvent != Event::EventID::None && _ActiveEvent != id)
		return false;

	//�͈͊O�A�N�Z�X���`�F�b�N�B
	try 
	{
		//�~�߂�B
		GetPlayer()->GetPlayerHpBar()->RenderDisable();
		//GetPlayer()->GetPlayerMpBar()->RenderDisable();
		GetPlayer()->PlayerStopEnable();

		GetCamera()->SetIsMove(false);
		//���s���̃C�x���g�̓Y�����ێ��B
		_ActiveEvent = id;

		//�֐����A�N�V�������H
		if (idx >= 0)
			_FuncList.at((int)id)(idx);
		else
			_ActionList.at((int)id - (int)Event::EventID::FunctionNum)();
	}
	//�͈͊O�A�N�Z�X�B
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
	if (_ActiveEvent != Event::EventID::None)
	{
		GetPlayer()->GetPlayerHpBar()->RenderEnable();
		//GetPlayer()->GetPlayerMpBar()->RenderEnable();
		GetPlayer()->PlayerStopDisable();
		GetCamera()->SetIsMove(true);
		_ActiveEvent = Event::EventID::None;
	}
}

void EventManager::AddEvent()
{
	//�V���b�v�B
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("Shop", 0);
	//�C�x���g�J�����B
	auto Ecamera = INSTANCE(GameObjectManager)->AddNew<EventCamera>("EventCamera", 8);
	Ecamera->SetNextCamera(GetCamera());
	
	StatusWindow* status = (StatusWindow*)INSTANCE(GameObjectManager)->FindObject("StatusWindow");
	HistoryBook* book = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");
	
	WorldMap* map = static_cast<WorldMap*>(INSTANCE(GameObjectManager)->FindObject("WorldMap"));

	//�֐���ǉ��B

	//�V���b�v���J�������B
	_FuncList.push_back(std::bind(&Shop::OpenShop, shop, std::placeholders::_1));
	//�C�x���g�J�������N�����鏈���B
	_FuncList.push_back(std::bind(&EventCamera::Excute, Ecamera, std::placeholders::_1));


	//

	//�X�e�[�^�X�E�B���h�E�̊J�����B
	_ActionList.push_back([status]
	{
		status->Execute();
	});
	//���j���̊J�����B
	_ActionList.push_back([book]
	{
		book->SetActive(!book->GetActive(), true);
	});

	// ���[���h�}�b�v��ǉ��B
	_ActionList.push_back([map]
	{
		map->Open();
	});
}