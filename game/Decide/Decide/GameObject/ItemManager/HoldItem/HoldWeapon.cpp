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
	_Crt = static_cast<Item::WeaponInfo*>(_Info)->Dex;

	//����̃����N���Z�o�B
	RankSelect(ParamRaitoMass());
}