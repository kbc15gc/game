#include "stdafx.h"
#include "CharacterParameter.h"

void CharacterParameter::ParamInit(int hp, int mp, int atk, int def, int dex, int agi)
{
	//パラメーター設定。
	_Param[Param::HP] = hp;		//ヒットポイント
	_Param[Param::MP] = mp;		//マジックポイント
	_Param[Param::ATK] = atk;		//攻撃力
	_Param[Param::DEF] = def;		//防御力
	_Param[Param::DEX] = dex;		//命中力
	_Param[Param::AGI] = agi;		//回避力
}
