#include "stdafx.h"
#include "GameObject\ItemManager\HoldItem\HoldEquipment.h"
#include "GameObject\Inventory\Inventory.h"

HoldEquipment::HoldEquipment(char* name) :HoldItemBase(name)
{
}


HoldEquipment::~HoldEquipment()
{

}

void HoldEquipment::ConfigLoadData(Hold::HoldEquipInfo* info) {
	_ConfigLoadDataSubClass(info);
	_IsEquip = info->_IsEquip;
	RankSelect(ParamRaitoMass());
	_isLoad = true;
}
