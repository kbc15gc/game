#include "stdafx.h"
#include "CharacterParameter.h"

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

void CharacterParameter::DamageMass(int atk, int def)
{
	int Damage = 0;

	//ダメージ=攻撃力-(0.5(補正値的な何かにそのうち置き換えて)*防御力)。
	Damage = atk - (0.5f*def);

	//体力を計算したダメージ分減算。
	SubParam(Param::HP, Damage);

	//体力が0以下になったので死んだ。
	if (_Param[Param::HP] <= 0)
	{
		_DeathFlag = true;
	}

}