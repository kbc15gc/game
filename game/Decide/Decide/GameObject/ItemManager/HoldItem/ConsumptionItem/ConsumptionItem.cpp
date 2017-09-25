#include "stdafx.h"
#include "ConsumptionItem.h"
#include "GameObject\Player\Player.h"


ConsumptionItem::ConsumptionItem(Item::BaseInfo* info) :HoldItemBase(info)
{
	_UseTarget = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}


ConsumptionItem::~ConsumptionItem()
{
}

//アイテムを使う。
void ConsumptionItem::UseItem(Item::ItemInfo* item) {

	//使うアイテムの情報を設定。
	_UseTarget->SetBuff(item->Recovery, item->AtkBuff, item->DefBuff, item->SpeedBuff);
}
