#include "stdafx.h"
#include "HoldEquipment.h"

HoldEquipment::HoldEquipment(Item::BaseInfo* info) :HoldItemBase(info)
{
	//基底値で武器、防具のパラメーターを作成。
	CreateOriginParam();
}


HoldEquipment::~HoldEquipment()
{

}
