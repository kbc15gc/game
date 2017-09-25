#include "stdafx.h"
#include "HoldEquipment.h"

HoldEquipment::HoldEquipment(char* name) :HoldItemBase(name)
{
}


HoldEquipment::~HoldEquipment()
{

}

void HoldEquipment::ConfigLoadData(HoldEquipInfo* info) {
	_ConfigLoadDataSubClass(info);
	_IsEquip = info->_IsEquip;
	RankSelect(ParamRaitoMass());
	_isLoad = true;
}
