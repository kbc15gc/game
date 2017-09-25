#include "stdafx.h"
#include "HoldArmor.h"

HoldArmor::HoldArmor(char* name) :HoldEquipment(name)
{
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
