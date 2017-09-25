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
	//HPが0以下になったので死んだ。
	if (_Info[Param::HP].param <= 0)
	{
		_DeathFlag = true;
	}
}

unsigned short CharacterParameter::ReciveDamage(unsigned short defaultDamage, bool isMagic, HoldArmor* armor, unsigned short defidx) {
	int damage = ReceiveDamageMass(defaultDamage, isMagic, armor, defidx);
	//ダメージ分HPを減算。
	_SubParam(Param::HP, damage);
	return damage;
}

unsigned short CharacterParameter::ReceiveDamageMass(unsigned short defaultDamage, bool isMagic, HoldArmor* armor, unsigned short defidx)
{
	//こちらの防御力も考慮したダメージ = 相手の与ダメージ-((防御力 + 装備品の防御力) * 属性的なやつ * キャラクターの行動による防御率)。
	float element = 1.0f;// 属性による補正的なやつ(とりあえず作るだけ作っとく)※暫定処理。

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
	unsigned short critMax = 100;	// クリティカル率最大。
	unsigned short crit = _Info[Param::DEX].param;// クリティカル率。

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

	// キャラの攻撃力に武器の攻撃力を加算。
	damage += weaponDamage;

	if (crit > 0) {
		unsigned short work = critMax / crit;
		if (work > 100) {
			work = 100;
		}

		if (rand() % work == 0) {
			// クリティカル。
			short crt = _Info[Param::CRT].param;	// キャラのクリティカル。
			if(weapon) {
				// 武器のクリティカル威力を加算。
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
		// 最大HPを超えた。

		_Info[Param::HP].param = _Info[Param::HP].originParam;
	}
	_Info[Param::HP].param = hp;
}

void CharacterParameter::HeelMP(unsigned short value) {
	unsigned short mp = _Info[Param::MP].param;
	mp += value;
	if (mp >= _Info[Param::MP].originParam) {
		// 最大HPを超えた。

		_Info[Param::MP].param = _Info[Param::MP].originParam;
	}
	_Info[Param::MP].param = mp;
}

void CharacterParameter::Buff(Param idx, unsigned short percentage, float time) {
	if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
		// バフとデバフに対応していないものは無視。
		return;
	}

	if (_Info[idx].buffPercentage <= percentage) {
		// 新しい効果値のほうが強力。

		_Info[idx].buffPercentage = percentage;	// 上書き。
		_Info[idx].buffInterval = time;	// 効果時間を再設定。
		_Info[idx].buffCounter = 0.0f;	// カウンターを初期化。
		_UpdateParam(idx);
	}
}

void CharacterParameter::BuffClear(Param idx) {
	if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
		// バフとデバフに対応していないものは無視。
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
		// バフとデバフに対応していないものは無視。
		return;
	}

	if (_Info[idx].debuffPercentage <= percentage) {
		// 新しい効果値のほうが強力。

		_Info[idx].debuffPercentage = percentage;	// 上書き。
		_Info[idx].debuffInterval = time;	// 効果時間を再設定。
		_Info[idx].debuffCounter = 0.0f;	// カウンターを初期化。
		_UpdateParam(idx);
	}
}

void CharacterParameter::DebuffClear(Param idx) {
	if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
		// バフとデバフに対応していないものは無視。
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
	_Info[idx].param = _Info[idx].originParam;	// 基礎パラメータ。
	
	_Info[idx].param += _Info[idx].originParam * static_cast<float>(_Info[idx].buffPercentage) * 0.01f;	// バフ値を加算。
	_Info[idx].param -= _Info[idx].originParam * static_cast<float>(_Info[idx].debuffPercentage) * 0.01f;	// デバフ値を減算。
}
