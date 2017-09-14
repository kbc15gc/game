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
	//パラメーター設定。
	_Param[Param::HP]		= hp;		//ヒットポイント。
	_Param[Param::MAXHP]	= maxhp;	//ヒットポイント。
	_Param[Param::MP]		= mp;		//マジックポイント。
	_Param[Param::MAXMP]	= maxmp;	//ヒットポイント。
	_Param[Param::ATK]		= atk;		//攻撃力。
	_Param[Param::MAT]		= mat;		//魔法攻撃力。
	_Param[Param::DEF]		= def;		//防御力。
	_Param[Param::MDE]		= mde;		//魔法防御力。
	_Param[Param::DEX]		= dex;		//命中力。
	_Param[Param::LV]		= lv;		//レベル。
	_Param[Param::EXP]		= exp;		//現在の経験値。
	_Param[Param::DROPEXP]	= dropexp;	//落とす経験値。
	_Param[Param::MONEY]	= money;	//所持金。
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
	//HPが0以下になったので死んだ。
	if (_Param[Param::HP] <= 0)
	{
		_DeathFlag = true;
	}
}

int CharacterParameter::ReciveDamage(int defaultDamage, bool isMagic, HoldArmor* armor, int defidx) {
	int damage = ReceiveDamageMass(defaultDamage, isMagic, armor, defidx);
	//ダメージ分HPを減算。
	SubParam(Param::HP, damage);
	return damage;
}

int CharacterParameter::ReceiveDamageMass(int defaultDamage, bool isMagic, HoldArmor* armor, int defidx)
{
	//こちらの防御力も考慮したダメージ = 相手の与ダメージ-((防御力 + 装備品の防御力) * 属性的なやつ * キャラクターの行動による防御率)。
	float element = 1.0f;// 属性による補正的なやつ(とりあえず作るだけ作っとく)※暫定処理。

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
	int critMax = 100;	// クリティカル率最大。
	int crit = _Param[Param::DEX];// クリティカル率。

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

	// キャラの攻撃力に武器の攻撃力を加算。
	damage += weaponDamage;

	if (crit > 0) {
		int work = critMax / crit;
		if (work > 100) {
			work = 100;
		}

		if (rand() % work == 0) {
			// クリティカル。
			damage += damage * static_cast<Item::WeaponInfo*>(weapon->GetInfo())->CriticalDamage;
		}
	}
	return damage * atk;
}

void CharacterParameter::LevelUP(int lvupexp,int hp,int mp,int atk,int mat, int def,int mde, int dex)
{
	//レベルアップに必要な経験値を超えるとレベルアップする。
	
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