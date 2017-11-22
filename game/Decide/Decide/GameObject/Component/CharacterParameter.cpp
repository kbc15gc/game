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
	// バフやデバフの状況を更新。
	_UpdateInfo();
}

int CharacterParameter::ReciveDamage(const DamageInfo& info, HoldArmor* armor, int defidx) {
	//ダメージ分HPを減算。
	return ReciveDamageThrough(ReceiveDamageMass(info, armor, defidx));
}

int CharacterParameter::ReceiveDamageMass(const DamageInfo& info, HoldArmor* armor, int defidx)
{
	if (info.isThrough) {
		return info.value;	// 貫通攻撃なのでそのままの値。
	}

	//こちらの防御力も考慮したダメージ = 相手の与ダメージ-((防御力 + 装備品の防御力) * 属性的なやつ * キャラクターの行動による防御率)。
	float element = 1.0f;// 属性による補正的なやつ(とりあえず作るだけ作っとく)※暫定処理。

	int def;

	if (info.isMagic) {
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

	int damage = max(0, info.value - (def * element * defidx));

	return damage;
}

int CharacterParameter::ReciveDamageThrough(int damage) {
	if (damage > 0) {
		int hp = GetParam(Param::HP) - damage;
		if (hp <= 0) {
			_Info[Param::HP].param = 0;
			//HPが0以下になったので死んだ。
			_DeathFlag = true;
		}
		else {
			_Info[Param::HP].param = hp;
		}
	}
	return damage;
}

unique_ptr<CharacterParameter::DamageInfo> CharacterParameter::GiveDamageMass(bool isMagic, bool isThrough, HoldWeapon* weapon, int percentage)
{
	int damage = 0;
	int weaponDamage = 0;
	int critMax = 100;	// クリティカル率最大。
	int crit = _Info[Param::DEX].param;// クリティカル率。

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

			//@todo for debug
			//critの値がランクによって100を超える場合があるため。
			//100以上の場合は100にします。
			//不具合が出た場合変更します。
			if (crit > 100)
			{
				crit = 100;
			}
		}
	}

	// キャラの攻撃力に武器の攻撃力を加算。
	damage += weaponDamage;

	unique_ptr<DamageInfo> info(new DamageInfo);

	if (crit > 0) {
		int work = critMax / crit;
		if (work > 100) {
			work = 100;
		}

		if (rand() % work == 0) {
			// クリティカル。

			info->isCritical = true;
			short crt = _Info[Param::CRT].param;	// キャラのクリティカル。
			if(weapon) {
				// 武器のクリティカル威力を加算。
				crt += weapon->GetCrt();
			}
			damage += static_cast<int>(damage * static_cast<float>(crt) * 0.01f);
		}
	}

	info->value = static_cast<int>(damage * (static_cast<float>(percentage) * 0.01f));
	info->isMagic = isMagic;
	info->isThrough = isThrough;
	return move(info);
}

bool CharacterParameter::HeelHP(int value) {
	int hp = _Info[Param::HP].param;
	if (value == 0 || hp >= _Info[Param::HP].originParam) {
		// 回復不可。

		return false;
	}

	hp += value;
	if (hp >= _Info[Param::HP].originParam) {
		// 最大HPを超えた。

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
		// 回復不可。

		return false;
	}

	mp += value;
	if (mp >= _Info[Param::MP].originParam) {
		// 最大HPを超えた。

		_Info[Param::MP].param = _Info[Param::MP].originParam;
	}
	else {
		_Info[Param::MP].param = mp;
	}
	return true;
}

void CharacterParameter::Buff(Param idx, int percentage, float time) {
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
	for (int idx = 0; idx < static_cast<int>(_Info.size()); idx++) {
		BuffClear(static_cast<Param>(idx));
	}
}


void CharacterParameter::Debuff(Param idx, int percentage, float time) {
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
	for (int idx = 0; idx < static_cast<int>(_Info.size()); idx++) {
		DebuffClear(static_cast<Param>(idx));
	}
}

void CharacterParameter::_UpdateParam(Param idx) {
	_Info[idx].param = _Info[idx].originParam;	// 基礎パラメータ。
	
	_Info[idx].param += static_cast<int>(_Info[idx].originParam * static_cast<float>(_Info[idx].buffPercentage) * 0.01f);	// バフ値を加算。
	_Info[idx].param -= static_cast<int>(_Info[idx].originParam * static_cast<float>(_Info[idx].debuffPercentage) * 0.01f);	// デバフ値を減算。
}

void CharacterParameter::_UpdateInfo() {
	for (int idx = 0; idx < Param::MAX;idx++) {
		if (_Info[idx].buffCounter >= _Info[idx].buffInterval) {
			// バフの効果時間が過ぎた。

			BuffClear(static_cast<Param>(idx));
		}
		else {
			_Info[idx].buffCounter += Time::DeltaTime();
		}

		if (_Info[idx].debuffCounter >= _Info[idx].debuffInterval) {
			// デバフの効果時間が過ぎた。

			DebuffClear(static_cast<Param>(idx));
		}
		else {
			_Info[idx].debuffCounter += Time::DeltaTime();
		}
	}
}
