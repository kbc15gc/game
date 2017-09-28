#include "stdafx.h"
#include "HoldWeapon.h"


HoldWeapon::HoldWeapon(char* name) :HoldEquipment(name)
{
	//最初は装備していない。
	SetIsEquipFalse();
}

//デストラクタ。
HoldWeapon::~HoldWeapon()
{

}

//武器のパラメーターをランダム差分を使って算出。
void HoldWeapon::CreateRandParam() 
{
	//ランダムパラメーターを算出。
	RndAtkMass();
	RndMAtkMass();
	RndCrtMass();

	//武器のランクを算出。
	RankSelect(ParamRaitoMass());
}

//武器のパラメーターを基準値で設定。
void HoldWeapon::CreateOriginParam() {

	//基準値設定。
	_Atk = static_cast<Item::WeaponInfo*>(_Info)->Atk;
	_MagicAtk = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;
	_Dex = static_cast<Item::WeaponInfo*>(_Info)->Dex;

	//武器のランクを算出。
	RankSelect(ParamRaitoMass());
}

void HoldWeapon::_ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) {
	// ランダム差分設定。
	_AtkRnd = static_cast<Hold::HoldWeaponInfo*>(info)->_AtkRnd;
	_MAtkRnd = static_cast<Hold::HoldWeaponInfo*>(info)->_MAtkRnd;
	_DexRnd = static_cast<Hold::HoldWeaponInfo*>(info)->_DexRnd;
	// 合計パラメータ設定。
	_Atk = static_cast<Item::WeaponInfo*>(_Info)->Atk + _AtkRnd;
	_MagicAtk = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk + _MAtkRnd;
	_Dex = static_cast<Item::WeaponInfo*>(_Info)->Dex + _DexRnd;
}
