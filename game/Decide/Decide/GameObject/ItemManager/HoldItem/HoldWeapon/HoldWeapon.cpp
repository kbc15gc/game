#include "stdafx.h"
#include "HoldWeapon.h"


HoldWeapon::HoldWeapon(Item::BaseInfo* info):HoldItemBase(info)
{
	// 物理攻撃力のランダム差分算出。
	int baseParam = static_cast<Item::WeaponInfo*>(info)->Atk;
	int rnd = GetRand_S50to100();// -50から100の値をランダムで取得。
	float raito = static_cast<float>(rnd) * 0.01f;
	_AtkRnd = baseParam * raito;
	_Atk = baseParam + _AtkRnd;

	// 魔法攻撃力のランダム差分算出。
	baseParam = static_cast<Item::WeaponInfo*>(info)->MagicAtk;
	rnd = GetRand_S50to100();	// -50から100の値をランダムで取得。
	raito = static_cast<float>(rnd) * 0.01f;
	_MAtkRnd = baseParam * raito;
	_MagicAtk = baseParam + _MAtkRnd;

	//クリティカルのランダム差分算出。
	baseParam = static_cast<Item::WeaponInfo*>(info)->Dex;
	rnd = GetRand_S50to100();
	raito = static_cast<float>(rnd) * 0.01f;
	_CrtRnd = baseParam * raito;
	_Crt = baseParam + _CrtRnd;

	RankSelect(ParamRaitoMass());
	
}


HoldWeapon::~HoldWeapon()
{
}
