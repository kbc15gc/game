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
	//�h��̃����N���Z�o�B
	RankSelect();

	//�����_���p�����[�^�[��ݒ�B

	// ���v�����l���e�p�����[�^�ɕ��U�B
	int max = 10;
	int rnd = rand() % max;
	float frnd = static_cast<float>(rnd) * 0.1f;
	RndDefMass(GetRevision() * frnd);

	max -= rnd;
	frnd = static_cast<float>(max) * 0.1f;
	RndMDef(GetRevision() * frnd);
}

//�h��̃p�����[�^�[����l�Őݒ�B
void HoldArmor::CreateOriginParam() {
	//��l�ݒ�B
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;

	_DefRnd = 0;
	_MDefRnd = 0;

	//�h��̃����N��ݒ�B
	_Rank = Rank::C;
	_Revision = 0.0f;
}

void HoldArmor::_ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) {
	// �����_�������ݒ�B
	_DefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_DefRnd;
	_MDefRnd = static_cast<Hold::HoldArmorInfo*>(info)->_MDefRnd;
	// ���v�p�����[�^�ݒ�B
	_Def = static_cast<Item::ArmorInfo*>(_Info)->Def + _DefRnd;
	_MagicDef = static_cast<Item::ArmorInfo*>(_Info)->MagicDef + _MDefRnd;
}