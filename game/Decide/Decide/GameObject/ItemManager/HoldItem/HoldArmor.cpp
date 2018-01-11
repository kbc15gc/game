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
	//防具のランクを算出。
	RankSelect();

	//ランダムパラメーターを設定。

	// 合計差分値を各パラメータに分散。
	int max = 10;
	int rnd = rand() % max;
	float frnd = static_cast<float>(rnd) * 0.1f;
	RndDefMass(GetRevision() * frnd);

	max -= rnd;
	frnd = static_cast<float>(max) * 0.1f;
	RndMDef(GetRevision() * frnd);
}

//防具のパラメーターを基準値で設定。
void HoldArmor::CreateOriginParam() {
	//基準値設定。
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;

	_DefRnd = 0;
	_MDefRnd = 0;

	//防具のランクを設定。
	_Rank = Rank::C;
	_Revision = 0.0f;
}

void HoldArmor::_ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) {
	// ランダム差分設定。
	_DefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_DefRnd;
	_MDefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_MDefRnd;
	// 合計パラメータ設定。
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def + _DefRnd;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef + _MDefRnd;
}