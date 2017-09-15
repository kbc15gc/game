#include "stdafx.h"
#include "HoldArmor.h"

HoldArmor::HoldArmor(Item::BaseInfo* info):HoldItemBase(info)
{
	// 物理防御力のランダム差分算出。
	int baseParam = static_cast<Item::ArmorInfo*>(info)->Def;
	int rnd = GetRand_S50to100();// -50から100の値をランダムで取得。
	float raito = static_cast<float>(rnd) * 0.01f;
	_DefRnd = baseParam * raito;
	_Def = baseParam + _DefRnd;

	// 魔法防御力のランダム差分算出。
	baseParam = static_cast<Item::ArmorInfo*>(info)->MagicDef;
	rnd = GetRand_S50to100();	// -50から100の値をランダムで取得。
	raito = static_cast<float>(rnd) * 0.01f;
	_MDefRnd = baseParam * raito;
	_MagicDef = baseParam + _MDefRnd;

	RankSelect(ParamRaitoMass());
}


HoldArmor::~HoldArmor()
{
}
