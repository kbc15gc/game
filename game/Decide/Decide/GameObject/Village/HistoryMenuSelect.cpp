#include "stdafx.h"
#include "HistoryMenuSelect.h"
#include "fbEngine\_Object\_GameObject\Button.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryButton.h"

namespace
{
	Vector3 chippos[] =
	{
		Vector3(0,0,0),
		Vector3(1000.0f, 200.0f, 0.0f),
		Vector3(1000.0f, 350.0f, 0.0f),
		Vector3(1000.0f, 500.0f, 0.0f),
	};
}

HistoryMenuSelect::HistoryMenuSelect(const char * name) :
	GameObject(name)
{

}

HistoryMenuSelect::~HistoryMenuSelect()
{
}

void HistoryMenuSelect::Awake()
{
}

void HistoryMenuSelect::Start()
{
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

void HistoryMenuSelect::ChipSelect(ChipID id)
{
	//ボタン
	HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
	b->SetMenuButtonFlag(false);
	b->SetChipID(id);
	Vector3 pos = chippos[(int)id];
	b->transform->SetLocalPosition(pos);
	_MenuSelectObjects.push_back(b);

	//最初は表示しないためFALSE
	_SetMenuEnabel(false);
}