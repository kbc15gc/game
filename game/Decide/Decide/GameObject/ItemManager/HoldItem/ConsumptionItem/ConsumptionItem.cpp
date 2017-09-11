#include "stdafx.h"
#include "ConsumptionItem.h"
#include "GameObject\Player\Player.h"


ConsumptionItem::ConsumptionItem(const char* name) : HoldItemBase(name)
{
	Target= (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}


ConsumptionItem::~ConsumptionItem()
{
}

//アイテムを使う。
void ConsumptionItem::UseItem(Item::ItemInfo* item) {

	//使うアイテムの情報を設定。
	Target->SetBuff(item->Recovery, item->AtkBuff, item->DefBuff, item->SpeedBuff);
}
