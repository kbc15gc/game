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
//	//パラメーター設定。
//	_Param[Param::HP]		= hp;		//ヒットポイント。
//	_Param[Param::MAXHP]	= maxhp;	//ヒットポイント。
//	_Param[Param::MP]		= mp;		//マジックポイント。
//	_Param[Param::MAXMP]	= maxmp;	//ヒットポイント。
//	_Param[Param::ATK]		= atk;		//攻撃力。
//	_Param[Param::MAT]		= mat;		//魔法攻撃力。
//	_Param[Param::DEF]		= def;		//防御力。
//	_Param[Param::MDE]		= mde;		//魔法防御力。
//	_Param[Param::DEX]		= dex;		//器用度。
//	_Param[Param::CRT]		= crt;		//クリティカル威力。
//	_Param[Param::LV]		= lv;		//レベル。
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
			int crt = _Param[Param::CRT];	// キャラのクリティカル。
			if(weapon) {
				// 武器のクリティカル威力を加算。
				crt += static_cast<Item::WeaponInfo*>(weapon->GetInfo())->CriticalDamage;
			}
			damage += damage * static_cast<float>(crt) * 0.01f;
		}
	}
	return damage * atk;
}

