#include "stdafx.h"
#include "HoldWeapon.h"


HoldWeapon::HoldWeapon(Item::BaseInfo* info) :HoldEquipment(info)
{
	
}


HoldWeapon::~HoldWeapon()
{

}

void HoldWeapon::CreateRandParam() 
{
	
	RndAtkMass();
	RndMAtkMass();
	RndCrtMass();
	RankSelect(ParamRaitoMass());
}

void HoldWeapon::CreateOriginParam() {
	_Atk = static_cast<Item::WeaponInfo*>(_Info)->Atk;
	_MagicAtk = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->Dex;
	RankSelect(ParamRaitoMass());
}