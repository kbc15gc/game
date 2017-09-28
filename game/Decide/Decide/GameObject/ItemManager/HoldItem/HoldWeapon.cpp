#include "stdafx.h"
#include "HoldWeapon.h"


HoldWeapon::HoldWeapon(char* name) :HoldEquipment(name)
{
	//�ŏ��͑������Ă��Ȃ��B
	SetIsEquipFalse();
}

//�f�X�g���N�^�B
HoldWeapon::~HoldWeapon()
{

}

//����̃p�����[�^�[�������_���������g���ĎZ�o�B
void HoldWeapon::CreateRandParam() 
{
	//�����_���p�����[�^�[���Z�o�B
	RndAtkMass();
	RndMAtkMass();
	RndCrtMass();

	//����̃����N���Z�o�B
	RankSelect(ParamRaitoMass());
}

//����̃p�����[�^�[����l�Őݒ�B
void HoldWeapon::CreateOriginParam() {

	//��l�ݒ�B
	_Atk = static_cast<Item::WeaponInfo*>(_Info)->Atk;
	_MagicAtk = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;
	_Dex = static_cast<Item::WeaponInfo*>(_Info)->Dex;

	//����̃����N���Z�o�B
	RankSelect(ParamRaitoMass());
}

void HoldWeapon::_ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) {
	// �����_�������ݒ�B
	_AtkRnd = static_cast<Hold::HoldWeaponInfo*>(info)->_AtkRnd;
	_MAtkRnd = static_cast<Hold::HoldWeaponInfo*>(info)->_MAtkRnd;
	_DexRnd = static_cast<Hold::HoldWeaponInfo*>(info)->_DexRnd;
	// ���v�p�����[�^�ݒ�B
	_Atk = static_cast<Item::WeaponInfo*>(_Info)->Atk + _AtkRnd;
	_MagicAtk = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk + _MAtkRnd;
	_Dex = static_cast<Item::WeaponInfo*>(_Info)->Dex + _DexRnd;
}
