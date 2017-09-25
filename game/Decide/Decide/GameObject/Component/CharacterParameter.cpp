#include "stdafx.h"
#include "CharacterParameter.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"

#ifdef _DEBUG
const wchar_t* CharacterParameter::ENUM_NAME[] = {
	L"HP    ",
	L"MP    ",
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
	_Info = vector<ParamInfo>(Param::MAX);
}

void CharacterParameter::ParamReset(int param[Param::MAX]) {
	for (short idx = 0; idx < Param::MAX; idx++) {
		_Info[idx].originParam = static_cast<unsigned short>(param[idx]);
		_UpdateParam(static_cast<Param>(idx));
	}
}

void CharacterParameter::ParamReset(const vector<int>& param) {
	for (short idx = 0; idx < Param::MAX; idx++) {
		_Info[idx].originParam = static_cast<unsigned short>(param[idx]);
		_UpdateParam(static_cast<Param>(idx));
	}
}

void CharacterParameter::Update() {
	//HP��0�ȉ��ɂȂ����̂Ŏ��񂾁B
	if (_Info[Param::HP].param <= 0)
	{
		_DeathFlag = true;
	}
}

unsigned short CharacterParameter::ReciveDamage(unsigned short defaultDamage, bool isMagic, HoldArmor* armor, unsigned short defidx) {
	int damage = ReceiveDamageMass(defaultDamage, isMagic, armor, defidx);
	//�_���[�W��HP�����Z�B
	_SubParam(Param::HP, damage);
	return damage;
}

unsigned short CharacterParameter::ReceiveDamageMass(unsigned short defaultDamage, bool isMagic, HoldArmor* armor, unsigned short defidx)
{
	//������̖h��͂��l�������_���[�W = ����̗^�_���[�W-((�h��� + �����i�̖h���) * �����I�Ȃ�� * �L�����N�^�[�̍s���ɂ��h�䗦)�B
	float element = 1.0f;// �����ɂ��␳�I�Ȃ��(�Ƃ肠������邾������Ƃ�)���b�菈���B

	short def;

	if (isMagic) {
		def = _Info[Param::MDE].param;
		if (armor) {
			def += armor->GetMagicDef();
		}
	}
	else {
		def = _Info[Param::DEF].param;
		if (armor) {
			def += armor->GetDef();
		}
	}

	short damage = max(0, defaultDamage - (def * element * defidx));

	return damage;
}

unsigned short CharacterParameter::GiveDamageMass(bool isMagic, HoldWeapon* weapon, unsigned short atk)
{
	unsigned short damage = 0;
	unsigned short weaponDamage = 0;
	unsigned short critMax = 100;	// �N���e�B�J�����ő�B
	unsigned short crit = _Info[Param::DEX].param;// �N���e�B�J�����B

	if (isMagic) {
		damage = _Info[Param::MAT].param;
		if (weapon) {
			weaponDamage = weapon->GetMagicAtk();
			crit += weapon->GetCrt();
		}
	}
	else {
		damage = _Info[Param::ATK].param;
		if (weapon) {
			weaponDamage = weapon->GetAtk();
			crit += weapon->GetCrt();
		}
	}

	// �L�����̍U���͂ɕ���̍U���͂����Z�B
	damage += weaponDamage;

	if (crit > 0) {
		unsigned short work = critMax / crit;
		if (work > 100) {
			work = 100;
		}

		if (rand() % work == 0) {
			// �N���e�B�J���B
			short crt = _Info[Param::CRT].param;	// �L�����̃N���e�B�J���B
			if(weapon) {
				// ����̃N���e�B�J���З͂����Z�B
				crt += static_cast<Item::WeaponInfo*>(weapon->GetInfo())->CriticalDamage;
			}
			damage += damage * static_cast<float>(crt) * 0.01f;
		}
	}
	return damage * atk;
}

void CharacterParameter::HeelHP(unsigned short value) {
	unsigned short hp = _Info[Param::HP].param;
	hp += value;
	if (hp >= _Info[Param::HP].originParam) {
		// �ő�HP�𒴂����B

		_Info[Param::HP].param = _Info[Param::HP].originParam;
	}
	_Info[Param::HP].param = hp;
}

void CharacterParameter::HeelMP(unsigned short value) {
	unsigned short mp = _Info[Param::MP].param;
	mp += value;
	if (mp >= _Info[Param::MP].originParam) {
		// �ő�HP�𒴂����B

		_Info[Param::MP].param = _Info[Param::MP].originParam;
	}
	_Info[Param::MP].param = mp;
}

void CharacterParameter::Buff(Param idx, unsigned short percentage, float time) {
	if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
		// �o�t�ƃf�o�t�ɑΉ����Ă��Ȃ����͖̂����B
		return;
	}

	if (_Info[idx].buffPercentage <= percentage) {
		// �V�������ʒl�̂ق������́B

		_Info[idx].buffPercentage = percentage;	// �㏑���B
		_Info[idx].buffInterval = time;	// ���ʎ��Ԃ��Đݒ�B
		_Info[idx].buffCounter = 0.0f;	// �J�E���^�[���������B
		_UpdateParam(idx);
	}
}

void CharacterParameter::BuffClear(Param idx) {
	if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
		// �o�t�ƃf�o�t�ɑΉ����Ă��Ȃ����͖̂����B
		return;
	}

	_Info[idx].buffPercentage = 0;
	_Info[idx].buffInterval = 0.0f;
	_Info[idx].buffCounter = 0.0f;
	_UpdateParam(idx);
}

void CharacterParameter::BuffClearAll() {
	for (int idx = 0; idx < _Info.size(); idx++) {
		BuffClear(static_cast<Param>(idx));
	}
}


void CharacterParameter::Debuff(Param idx, unsigned short percentage, float time) {
	if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
		// �o�t�ƃf�o�t�ɑΉ����Ă��Ȃ����͖̂����B
		return;
	}

	if (_Info[idx].debuffPercentage <= percentage) {
		// �V�������ʒl�̂ق������́B

		_Info[idx].debuffPercentage = percentage;	// �㏑���B
		_Info[idx].debuffInterval = time;	// ���ʎ��Ԃ��Đݒ�B
		_Info[idx].debuffCounter = 0.0f;	// �J�E���^�[���������B
		_UpdateParam(idx);
	}
}

void CharacterParameter::DebuffClear(Param idx) {
	if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
		// �o�t�ƃf�o�t�ɑΉ����Ă��Ȃ����͖̂����B
		return;
	}

	_Info[idx].debuffPercentage = 0;
	_Info[idx].debuffInterval = 0.0f;
	_Info[idx].debuffCounter = 0.0f;
	_UpdateParam(idx);
}

void CharacterParameter::DebuffClearAll() {
	for (int idx = 0; idx < _Info.size(); idx++) {
		DebuffClear(static_cast<Param>(idx));
	}
}

void CharacterParameter::_UpdateParam(Param idx) {
	_Info[idx].param = _Info[idx].originParam;	// ��b�p�����[�^�B
	
	_Info[idx].param += _Info[idx].originParam * static_cast<float>(_Info[idx].buffPercentage) * 0.01f;	// �o�t�l�����Z�B
	_Info[idx].param -= _Info[idx].originParam * static_cast<float>(_Info[idx].debuffPercentage) * 0.01f;	// �f�o�t�l�����Z�B
}
