#include "stdafx.h"
#include "HoldArmor.h"

HoldArmor::HoldArmor(char* name) :HoldEquipment(name)
{
	//最初は装備していない。
	SetIsEquipFalse();
}

//デストラクタ。
HoldArmor::~HoldArmor()
{

}

//防具のパラメーターをランダムで算出。
void HoldArmor::CreateRandParam() 
{
	//ランダムパラメーターを算出。
	RndDefMass();
	RndMDef();

	//防具のランクを算出。
	RankSelect(ParamRaitoMass());
}

//防具のパラメーターを基準値で設定。
void HoldArmor::CreateOriginParam() {
	//基準値設定。
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;

	//防具のランクを算出。
	RankSelect(ParamRaitoMass());
}

void HoldArmor::_ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) {
	// ランダム差分設定。
	_DefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_DefRnd;
	_MDefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_MDefRnd;
	// 合計パラメータ設定。
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def + _DefRnd;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef + _MDefRnd;
}