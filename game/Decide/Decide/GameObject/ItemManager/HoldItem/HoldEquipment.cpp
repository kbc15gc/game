#include "stdafx.h"
#include "GameObject\ItemManager\HoldItem\HoldEquipment.h"
#include "GameObject\Inventory\Inventory.h"

HoldEquipment::HoldEquipment(char* name) :HoldItemBase(name)
{
}


HoldEquipment::~HoldEquipment()
{

}

void HoldEquipment::ConfigLoadData(Hold::HoldInfo* info) {
	_ConfigLoadDataSubClass(static_cast<Hold::HoldEquipInfo*>(info));
	_IsEquip = static_cast<Hold::HoldEquipInfo*>(info)->_IsEquip;
	_Rank = static_cast<Rank>(static_cast<Hold::HoldEquipInfo*>(info)->_Rank);
	_isLoad = true;
}
