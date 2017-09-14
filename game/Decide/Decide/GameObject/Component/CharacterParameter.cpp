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
	L"MONEY",
};
#endif

void CharacterParameter::Awake() {
	_Param = vector<int>(Param::MAX,0);
}

void CharacterParameter::ParamInit(int hp, int maxhp, int mp, int maxmp, int atk, int mat, int def, int mde, int dex, int lv, int exp, int dropexp,int money)
{
	//�p�����[�^�[�ݒ�B
	_Param[Param::HP]		= hp;		//�q�b�g�|�C���g�B
	_Param[Param::MAXHP]	= maxhp;	//�q�b�g�|�C���g�B
	_Param[Param::MP]		= mp;		//�}�W�b�N�|�C���g�B
	_Param[Param::MAXMP]	= maxmp;	//�q�b�g�|�C���g�B
	_Param[Param::ATK]		= atk;		//�U���́B
	_Param[Param::MAT]		= mat;		//���@�U���́B
	_Param[Param::DEF]		= def;		//�h��́B
	_Param[Param::MDE]		= mde;		//���@�h��́B
	_Param[Param::DEX]		= dex;		//�����́B
	_Param[Param::LV]		= lv;		//���x���B
	_Param[Param::EXP]		= exp;		//���݂̌o���l�B
	_Param[Param::DROPEXP]	= dropexp;	//���Ƃ��o���l�B
	_Param[Param::MONEY]	= money;	//�������B
}

void CharacterParameter::ParamInit(int param[Param::MAX]) {
	for (int idx = 0; idx < Param::MAX; idx++) {
		_Param[idx] = param[idx];
	}
}

void CharacterParameter::ParamInit(const vector<int>& param) {
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

int CharacterParameter::ReciveDamage(int defaultDamage, bool isMagic, HoldArmor* armor, int defidx) {
	int damage = ReceiveDamageMass(defaultDamage, isMagic, armor, defidx);
	//�_���[�W��HP�����Z�B
	SubParam(Param::HP, damage);
	return damage;
}

int CharacterParameter::ReceiveDamageMass(int defaultDamage, bool isMagic, HoldArmor* armor, int defidx)
{
	//������̖h��͂��l�������_���[�W = ����̗^�_���[�W-((�h��� + �����i�̖h���) * �����I�Ȃ�� * �L�����N�^�[�̍s���ɂ��h�䗦)�B
	float element = 1.0f;// �����ɂ��␳�I�Ȃ��(�Ƃ肠������邾������Ƃ�)���b�菈���B

	int def;

	if (isMagic) {
		def = _Param[Param::MDE];
		if (armor) {
			def += armor-
		}
	}
	else {
		def = _Param[Param::DEF];
		if (armor) {
			def += armor->
		}
	}

	int damage = max(0, defaultDamage - (def * element * defidx));

	return damage;
}

int CharacterParameter::GiveDamageMass(bool isMagic, HoldWeapon* weapon = nullptr, int atk = 1)
{
	int damage = 0;
	int weaponDamage = 0;
	int critMax = 100;	// �N���e�B�J�����ő�B
	int crit = _Param[Param::DEX];// �N���e�B�J�����B

	if (isMagic) {
		damage = _Param[Param::MAT];
		if (weapon) {
			weaponDamage = weapon->GetMagicAtk();
			crit += static_cast<Item::WeaponInfo*>(weapon->GetInfo())->Dex;
		}
	}
	else {
		damage = _Param[Param::ATK];
		if (weapon) {
			weaponDamage = weapon->GetAtk();
			crit += static_cast<Item::WeaponInfo*>(weapon->GetInfo())->Dex;
		}
	}

	// �L�����̍U���͂ɕ���̍U���͂����Z�B
	damage += weaponDamage;

	if (crit > 0) {
		int work = critMax / crit;
		if (work > 100) {
			work = 100;
		}

		if (rand() % work == 0) {
			// �N���e�B�J���B
			damage += damage * static_cast<Item::WeaponInfo*>(weapon->GetInfo())->CriticalDamage;
		}
	}
	return damage * atk;
}

void CharacterParameter::LevelUP(int lvupexp,int hp,int mp,int atk,int mat, int def,int mde, int dex)
{
	//���x���A�b�v�ɕK�v�Ȍo���l�𒴂���ƃ��x���A�b�v����B
	
		_Param[Param::LV] += 1;
		_Param[Param::EXP] = _Param[Param::EXP] - lvupexp;
		_Param[Param::HP] = hp;
		_Param[Param::MAXHP] = hp;
		_Param[Param::MP] = mp;
		_Param[Param::MAXMP] = mp;
		_Param[Param::ATK] = atk;
		_Param[Param::MAT] = mat;
		_Param[Param::DEF] = def;
		_Param[Param::MDE] = mde;
		_Param[Param::DEX] = dex;	
}