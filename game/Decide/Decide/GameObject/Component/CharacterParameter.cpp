#include "stdafx.h"
#include "CharacterParameter.h"

#ifdef _DEBUG
const wchar_t* CharacterParameter::ENUM_NAME[] = {
	L"HP    ",
	L"MAXHP ",
	L"MP    ",
	L"MAXMP ",
	L"ATK   ",
	L"MAT   ",
	L"DEF   ",
	L"MDE   ",
	L"DEX   ",
	L"CRT   ",
	L"LV	",
};
#endif

void CharacterParameter::Awake() {
	_Param = vector<int>(Param::MAX,0);
}

//void CharacterParameter::ParamInit(int hp, int maxhp, int mp, int maxmp, int atk, int mat, int def, int mde, int dex,int crt, int lv)
//{
//	//�p�����[�^�[�ݒ�B
//	_Param[Param::HP]		= hp;		//�q�b�g�|�C���g�B
//	_Param[Param::MAXHP]	= maxhp;	//�q�b�g�|�C���g�B
//	_Param[Param::MP]		= mp;		//�}�W�b�N�|�C���g�B
//	_Param[Param::MAXMP]	= maxmp;	//�q�b�g�|�C���g�B
//	_Param[Param::ATK]		= atk;		//�U���́B
//	_Param[Param::MAT]		= mat;		//���@�U���́B
//	_Param[Param::DEF]		= def;		//�h��́B
//	_Param[Param::MDE]		= mde;		//���@�h��́B
//	_Param[Param::DEX]		= dex;		//��p�x�B
//	_Param[Param::CRT]		= crt;		//�N���e�B�J���З́B
//	_Param[Param::LV]		= lv;		//���x���B
//}

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
			def += armor->GetMagicDef();
		}
	}
	else {
		def = _Param[Param::DEF];
		if (armor) {
			def += armor->GetDef();
		}
	}

	int damage = max(0, defaultDamage - (def * element * defidx));

	return damage;
}

int CharacterParameter::GiveDamageMass(bool isMagic, HoldWeapon* weapon, int atk)
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
			int crt = _Param[Param::CRT];	// �L�����̃N���e�B�J���B
			if(weapon) {
				// ����̃N���e�B�J���З͂����Z�B
				crt += static_cast<Item::WeaponInfo*>(weapon->GetInfo())->CriticalDamage;
			}
			damage += damage * static_cast<float>(crt) * 0.01f;
		}
	}
	return damage * atk;
}

