#include "stdafx.h"
#include "HoldEquipment.h"

HoldEquipment::HoldEquipment(Item::BaseInfo* info) :HoldItemBase(info)
{
	//���l�ŕ���A�h��̃p�����[�^�[���쐬�B
	CreateOriginParam();
}


HoldEquipment::~HoldEquipment()
{

}
