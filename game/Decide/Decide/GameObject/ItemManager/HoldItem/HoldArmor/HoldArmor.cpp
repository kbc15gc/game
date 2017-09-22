#include "stdafx.h"
#include "HoldArmor.h"

HoldArmor::HoldArmor(Item::BaseInfo* info) :HoldEquipment(info)
{
	SetIsEquipFalse();
}

HoldArmor::~HoldArmor()
{

}

void HoldArmor::CreateRandParam() 
{
	RndDefMass();
	RndMDef();
	RankSelect(ParamRaitoMass());
}

void HoldArmor::CreateOriginParam() {
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;
	RankSelect(ParamRaitoMass());
}
