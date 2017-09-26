#include "stdafx.h"
#include "HoldArmor.h"

HoldArmor::HoldArmor(char* name) :HoldEquipment(name)
{
	//�ŏ��͑������Ă��Ȃ��B
	SetIsEquipFalse();
}

//�f�X�g���N�^�B
HoldArmor::~HoldArmor()
{

}

//�h��̃p�����[�^�[�������_���ŎZ�o�B
void HoldArmor::CreateRandParam() 
{
	//�����_���p�����[�^�[���Z�o�B
	RndDefMass();
	RndMDef();

	//�h��̃����N���Z�o�B
	RankSelect(ParamRaitoMass());
}

//�h��̃p�����[�^�[����l�Őݒ�B
void HoldArmor::CreateOriginParam() {
	//��l�ݒ�B
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;

	//�h��̃����N���Z�o�B
	RankSelect(ParamRaitoMass());
}

void HoldArmor::_ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) {
	// �����_�������ݒ�B
	_DefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_DefRnd;
	_MDefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_MDefRnd;
	// ���v�p�����[�^�ݒ�B
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def + _DefRnd;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef + _MDefRnd;
}