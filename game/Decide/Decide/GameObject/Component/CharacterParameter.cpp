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
};
#endif

void CharacterParameter::Awake() {
	_Param = vector<int>(Param::MAX,0);
}

void CharacterParameter::ParamInit(int hp, int maxhp, int mp, int maxmp, int atk, int def, int dex, int agi)
{
	//パラメーター設定。
	_Param[Param::HP]	= hp;		//ヒットポイント。
	_Param[Param::MAXHP]= maxhp;	//ヒットポイント。
	_Param[Param::MP]	= mp;		//マジックポイント。
	_Param[Param::MAXMP]= maxmp;	//ヒットポイント。
	_Param[Param::ATK]	= atk;		//攻撃力。
	_Param[Param::DEF]	= def;		//防御力。
	_Param[Param::DEX]	= dex;		//命中力。
	_Param[Param::AGI]	= agi;		//回避力。
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

int CharacterParameter::ReciveDamage(int defaultDamage, int defidx, int Equipment) {
	int damage = ReceiveDamageMass(defaultDamage, defidx, Equipment);
	//ダメージ分HPを減算。
	SubParam(Param::HP, damage);
	return damage;
}

int CharacterParameter::ReceiveDamageMass(int defaultDamage, int defidx, int Equipment)
{
	//こちらの防御力も考慮したダメージ = 相手の与ダメージ-((防御力 + 装備品の防御力) * 属性的なやつ * キャラクターの行動による防御率)。
	float element = 1.0f;// 属性による補正的なやつ(とりあえず作るだけ作っとく)※暫定処理。
	int damage = max(0, defaultDamage - ((_Param[Param::DEF] + Equipment) * element * defidx));

	return damage;
}
