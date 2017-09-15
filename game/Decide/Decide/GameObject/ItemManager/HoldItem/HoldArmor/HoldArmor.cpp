#include "stdafx.h"
#include "HoldArmor.h"

HoldArmor::HoldArmor(Item::BaseInfo* info):HoldItemBase(info)
{
	// �����h��͂̃����_�������Z�o�B
	int baseParam = static_cast<Item::ArmorInfo*>(info)->Def;
	int rnd = GetRand_S50to100();// -50����100�̒l�������_���Ŏ擾�B
	float raito = static_cast<float>(rnd) * 0.01f;
	_DefRnd = baseParam * raito;
	_Def = baseParam + _DefRnd;

	// ���@�h��͂̃����_�������Z�o�B
	baseParam = static_cast<Item::ArmorInfo*>(info)->MagicDef;
	rnd = GetRand_S50to100();	// -50����100�̒l�������_���Ŏ擾�B
	raito = static_cast<float>(rnd) * 0.01f;
	_MDefRnd = baseParam * raito;
	_MagicDef = baseParam + _MDefRnd;

	RankSelect(ParamRaitoMass());
}


HoldArmor::~HoldArmor()
{
}
