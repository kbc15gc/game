#include "stdafx.h"
#include "CharacterParameter.h"

void CharacterParameter::ParamInit(int hp, int maxhp, int mp, int maxmp, int atk, int def, int dex, int agi)
{
	//�p�����[�^�[�ݒ�B
	_Param[Param::HP]	= hp;		//�q�b�g�|�C���g�B
	_Param[Param::MAXHP]= maxhp;	//�q�b�g�|�C���g�B
	_Param[Param::MP]	= mp;		//�}�W�b�N�|�C���g�B
	_Param[Param::MAXMP]= maxmp;	//�q�b�g�|�C���g�B
	_Param[Param::ATK]	= atk;		//�U���́B
	_Param[Param::DEF]	= def;		//�h��́B
	_Param[Param::DEX]	= dex;		//�����́B
	_Param[Param::AGI]	= agi;		//���́B
}

void CharacterParameter::Update() {
	//HP��0�ȉ��ɂȂ����̂Ŏ��񂾁B
	if (_Param[Param::HP] <= 0)
	{
		_DeathFlag = true;
	}
}

int CharacterParameter::ReciveDamage(int defaultDamage, int defidx, int Equipment) {
	int damage = ReceiveDamageMass(defaultDamage, defidx, Equipment);
	//�_���[�W��HP�����Z�B
	SubParam(Param::HP, damage);
	return damage;
}

int CharacterParameter::ReceiveDamageMass(int defaultDamage, int defidx, int Equipment)
{
	//������̖h��͂��l�������_���[�W = ����̗^�_���[�W-((�h��� + �����i�̖h���) * �����I�Ȃ�� * �L�����N�^�[�̍s���ɂ��h�䗦)�B
	float element = 1.0f;// �����ɂ��␳�I�Ȃ��(�Ƃ肠������邾������Ƃ�)���b�菈���B
	int damage = max(0, defaultDamage - ((_Param[Param::DEF] + Equipment) * element * defidx));

	return damage;
}