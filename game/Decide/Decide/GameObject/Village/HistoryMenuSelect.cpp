#include "stdafx.h"
#include "HistoryMenuSelect.h"
#include "fbEngine\_Object\_GameObject\Button.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryButton.h"

HistoryMenuSelect::HistoryMenuSelect(const char * name) :
	GameObject(name)
{

}

HistoryMenuSelect::~HistoryMenuSelect()
{
}

void HistoryMenuSelect::Start()
{
	Vector2 hasi(1000, 200);
	Vector2 interval(150, 150);

	//チップのセレクトメニュー
	{
		//火のチップメニュー
		{
			//火のボタン
			HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
			b->SetMenuButtonFlag(false);
			b->SetChipID(ChipID::FIRE);
			Vector3 pos = Vector3(1000.0f, 200.0f, 0.0f);
			b->transform->SetLocalPosition(pos);
			_MenuSelectObjects.push_back(b);
		}
		//鉄のチップメニュー
		{
			//鉄のボタン
			HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
			b->SetMenuButtonFlag(false);
			b->SetChipID(ChipID::IRON);
			Vector3 pos = Vector3(1000.0f, 350.0f, 0.0f);
			b->transform->SetLocalPosition(pos);
			_MenuSelectObjects.push_back(b);
		}
		//油のチップメニュー
		{
			//油のボタン
			HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
			b->SetMenuButtonFlag(false);
			b->SetChipID(ChipID::OIL);
			Vector3 pos = Vector3(1000.0f, 500.0f, 0.0f);
			b->transform->SetLocalPosition(pos);
			_MenuSelectObjects.push_back(b);
		}
	}

	_SetMenuEnabel(false);
}

void HistoryMenuSelect::Update()
{
	//Eキーの押下
	if (KeyBoardInput->isPush(DIK_E))
	{
		//フラグ反転
		_SetMenuEnabel(!_MenuEnabel);
	}

}

void HistoryMenuSelect::_SetMenuEnabel(const bool & enabel)
{
	_MenuEnabel = enabel;
	//メニューのオブジェクトすべてに適応する
	for each (GameObject* o in _MenuSelectObjects)
	{
		o->SetActive(_MenuEnabel);
	}
}