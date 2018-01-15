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
	//����̃����N���Z�o�B
	RankSelect();

	//�����_���p�����[�^�[��ݒ�B
	
	// ���v�����l���e�p�����[�^�ɕ��U�B
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
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->CriticalDamage;	// �N���e�B�J���З͂͌Œ�B
}

//����̃p�����[�^�[����l�Őݒ�B
void HoldWeapon::CreateOriginParam() {

	//��l�ݒ�B
	_Atk = static_cast<Item::WeaponInfo*>(_Info)->Atk;
	_MagicAtk = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;
	_Dex = static_cast<Item::WeaponInfo*>(_Info)->Dex;
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->CriticalDamage;

	_AtkRnd = 0;
	_MAtkRnd = 0;
	_DexRnd = 0;

	//����̃����N��ݒ�B
	_Rank = Rank::C;
	_Revision = 0.0f;
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
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->CriticalDamage;
}
