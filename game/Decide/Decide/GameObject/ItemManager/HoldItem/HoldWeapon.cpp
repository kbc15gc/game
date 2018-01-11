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
	//武器のランクを算出。
	RankSelect();

	//ランダムパラメーターを設定。
	
	// 合計差分値を各パラメータに分散。
	//int max = 10;
	//int rnd = rand() % max;
	//float frnd = static_cast<float>(rnd) * 0.1f;
	RndAtkMass(GetRevision()/* * frnd*/);

	//max -= rnd;
	//rnd = rand() % max;
	//frnd = static_cast<float>(rnd) * 0.1f;
	//RndMAtkMass(GetRevision() * frnd);

	//max -= rnd;
	//frnd = static_cast<float>(max) * 0.1f;
	//RndDexMass(GetRevision() * frnd);

	_Dex = static_cast<Item::WeaponInfo*>(_Info)->Dex;
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->CriticalDamage;	// クリティカル威力は固定。
}

//武器のパラメーターを基準値で設定。
void HoldWeapon::CreateOriginParam() {

	//基準値設定。
	_Atk = static_cast<Item::WeaponInfo*>(_Info)->Atk;
	_MagicAtk = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;
	_Dex = static_cast<Item::WeaponInfo*>(_Info)->Dex;
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->CriticalDamage;

	_AtkRnd = 0;
	_MAtkRnd = 0;
	_DexRnd = 0;

	//武器のランクを設定。
	_Rank = Rank::C;
	_Revision = 0.0f;
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
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->CriticalDamage;
}
