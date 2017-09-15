#include "stdafx.h"
#include "HoldWeapon.h"


HoldWeapon::HoldWeapon(Item::BaseInfo* info):HoldItemBase(info)
{
	// �����U���͂̃����_�������Z�o�B
	int baseParam = static_cast<Item::WeaponInfo*>(info)->Atk;
	int rnd = GetRand_S50to100();// -50����100�̒l�������_���Ŏ擾�B
	float raito = static_cast<float>(rnd) * 0.01f;
	_AtkRnd = baseParam * raito;
	_Atk = baseParam + _AtkRnd;

	// ���@�U���͂̃����_�������Z�o�B
	baseParam = static_cast<Item::WeaponInfo*>(info)->MagicAtk;
	rnd = GetRand_S50to100();	// -50����100�̒l�������_���Ŏ擾�B
	raito = static_cast<float>(rnd) * 0.01f;
	_MAtkRnd = baseParam * raito;
	_MagicAtk = baseParam + _MAtkRnd;

	//�N���e�B�J���̃����_�������Z�o�B
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
