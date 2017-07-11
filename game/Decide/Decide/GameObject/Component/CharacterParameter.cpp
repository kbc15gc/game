#include "stdafx.h"
#include "CharacterParameter.h"

#ifdef _DEBUG
const wchar_t* CharacterParameter::ENUM_NAME[] = {
	L"HP    ",
	L"MAXHP ",
	L"MP    ",
	L"MAXMP ",
	L"ATK   ",
	L"DEF   ",
	L"DEX   ",
	L"AGI   ",
	L"LV	",
	L"EXP	",
	L"DROPEXP",
};
#endif

void CharacterParameter::Awake() {
	_Param = vector<int>(Param::MAX,0);
}

void CharacterParameter::ParamInit(int hp, int maxhp, int mp, int maxmp, int atk, int def, int dex, int agi, int lv, int exp, int dropexp)
{
	//�p�����[�^�[�ݒ�B
	_Param[Param::HP]		= hp;		//�q�b�g�|�C���g�B
	_Param[Param::MAXHP]	= maxhp;	//�q�b�g�|�C���g�B
	_Param[Param::MP]		= mp;		//�}�W�b�N�|�C���g�B
	_Param[Param::MAXMP]	= maxmp;	//�q�b�g�|�C���g�B
	_Param[Param::ATK]		= atk;		//�U���́B
	_Param[Param::DEF]		= def;		//�h��́B
	_Param[Param::DEX]		= dex;		//�����́B
	_Param[Param::AGI]		= agi;		//���́B
	_Param[Param::LV]		= lv;		//���x���B
	_Param[Param::EXP]		= exp;		//���݂̌o���l�B
	_Param[Param::DROPEXP]	= dropexp;	//���Ƃ��o���l�B
}

void CharacterParameter::ParamInit(int param[Param::MAX]) {
	for (int idx = 0; idx < Param::MAX; idx++) {
		_Param[idx] = param[idx];
	}
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

void CharacterParameter::LevelUP(int lvupexp,int hp,int mp,int atk, int def, int dex, int agi)
{
	//���x���A�b�v�ɕK�v�Ȍo���l�𒴂���ƃ��x���A�b�v����B
	
		_Param[Param::LV] += 1;
		_Param[Param::EXP] = _Param[Param::EXP] - lvupexp;
		_Param[Param::HP] = hp;
		_Param[Param::MAXHP] = hp;
		_Param[Param::MP] = mp;
		_Param[Param::MAXMP] = mp;
		_Param[Param::ATK] = atk;
		_Param[Param::DEF] = def;
		_Param[Param::DEX] = dex;
		_Param[Param::AGI] = agi;
	
}