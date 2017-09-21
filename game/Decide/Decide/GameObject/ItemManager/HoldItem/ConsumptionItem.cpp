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

//�A�C�e�����g���B
void ConsumptionItem::UseItem(Item::ItemInfo* item) {

	//�g���A�C�e���̏���ݒ�B
	_UseTarget->SetBuff(item->Recovery, item->AtkBuff, item->DefBuff, item->SpeedBuff);
}
