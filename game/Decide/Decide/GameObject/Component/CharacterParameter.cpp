#include "stdafx.h"
#include "GameObject\Component\CharacterParameter.h"
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
		_Info[idx].originParam = param[idx];
		_UpdateParam(static_cast<Param>(idx));
	}
}

void CharacterParameter::ParamReset(const vector<int>& param) {
	for (short idx = 0; idx < Param::MAX; idx++) {
		_Info[idx].originParam = param[idx];
		_UpdateParam(static_cast<Param>(idx));
	}
}

void CharacterParameter::Update() {
	//HP��0�ȉ��ɂȂ����̂Ŏ��񂾁B
	if (_Info[Param::HP].param <= 0)
	{
		_DeathFlag = true;
	}

	// �o�t��f�o�t�̏󋵂��X�V�B
	_UpdateInfo();
}

int CharacterParameter::ReciveDamage(int defaultDamage, bool isMagic, HoldArmor* armor, int defidx) {
	//�_���[�W��HP�����Z�B
	return ReciveDamageThrough(ReceiveDamageMass(defaultDamage, isMagic, armor, defidx));
}

int CharacterParameter::ReceiveDamageMass(int defaultDamage, bool isMagic, HoldArmor* armor, int defidx)
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

int CharacterParameter::ReciveDamageThrough(int damage) {
	if (damage > 0) {
		int hp = GetParam(Param::HP) - damage;
		if (hp < 0) {
			_Info[Param::HP].param = 0;
		}
		else {
			_Info[Param::HP].param = hp;
		}
	}
	return damage;
}

unique_ptr<CharacterParameter::GiveDamageInfo> CharacterParameter::GiveDamageMass(bool isMagic, HoldWeapon* weapon, int atk)
{
	int damage = 0;
	int weaponDamage = 0;
	int critMax = 100;	// �N���e�B�J�����ő�B
	int crit = _Info[Param::DEX].param;// �N���e�B�J�����B

	if (isMagic) {
		damage = _Info[Param::MAT].param;
		if (weapon) {
			weaponDamage = weapon->GetMagicAtk();
			crit += weapon->GetDex();
		}
	}
	else {
		damage = _Info[Param::ATK].param;
		if (weapon) {
			weaponDamage = weapon->GetAtk();
			crit += weapon->GetDex();
		}
	}

	// �L�����̍U���͂ɕ���̍U���͂����Z�B
	damage += weaponDamage;

	unique_ptr<GiveDamageInfo> info(new GiveDamageInfo);

	if (crit > 0) {
		int work = critMax / crit;
		if (work > 100) {
			work = 100;
		}

		if (rand() % work == 0) {
			// �N���e�B�J���B

			info->isCritical = true;
			short crt = _Info[Param::CRT].param;	// �L�����̃N���e�B�J���B
			if(weapon) {
				// ����̃N���e�B�J���З͂����Z�B
				crt += weapon->GetCrt();
			}
			damage += damage * static_cast<float>(crt) * 0.01f;
		}
	}

	info->value = damage * atk;
	return move(info);
}

bool CharacterParameter::HeelHP(int value) {
	int hp = _Info[Param::HP].param;
	if (value == 0 || hp >= _Info[Param::HP].originParam) {
		// �񕜕s�B

		return false;
	}

	hp += value;
	if (hp >= _Info[Param::HP].originParam) {
		// �ő�HP�𒴂����B

		_Info[Param::HP].param = _Info[Param::HP].originParam;
	}
	else {
		_Info[Param::HP].param = hp;
	}
	return true;
}

bool CharacterParameter::HeelMP(int value) {
	int mp = _Info[Param::MP].param;
	if (value == 0 || mp >= _Info[Param::MP].originParam) {
		// �񕜕s�B

		return false;
	}

	mp += value;
	if (mp >= _Info[Param::MP].originParam) {
		// �ő�HP�𒴂����B

		_Info[Param::MP].param = _Info[Param::MP].originParam;
	}
	else {
		_Info[Param::MP].param = mp;
	}
	return true;
}

void CharacterParameter::Buff(Param idx, int percentage, float time) {
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


void CharacterParameter::Debuff(Param idx, int percentage, float time) {
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

void CharacterParameter::_UpdateInfo() {
	for (int idx = 0; idx < Param::MAX;idx++) {
		if (_Info[idx].buffCounter >= _Info[idx].buffInterval) {
			// �o�t�̌��ʎ��Ԃ��߂����B

			BuffClear(static_cast<Param>(idx));
		}
		else {
			_Info[idx].buffCounter += Time::DeltaTime();
		}

		if (_Info[idx].debuffCounter >= _Info[idx].debuffInterval) {
			// �f�o�t�̌��ʎ��Ԃ��߂����B

			DebuffClear(static_cast<Param>(idx));
		}
		else {
			_Info[idx].debuffCounter += Time::DeltaTime();
		}
	}
}
