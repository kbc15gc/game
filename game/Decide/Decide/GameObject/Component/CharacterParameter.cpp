#include "stdafx.h"
#include "CharacterParameter.h"

void CharacterParameter::ParamInit(int hp, int mp, int atk, int def, int dex, int agi)
{
	//パラメーター設定。
	_HP = hp;		//ヒットポイント
	_MP = mp;		//マジックポイント
	_ATK = atk;		//攻撃力
	_DEF = def;		//防御力
	_DEX = dex;		//命中力
	_AGI = agi;		//回避力
}
