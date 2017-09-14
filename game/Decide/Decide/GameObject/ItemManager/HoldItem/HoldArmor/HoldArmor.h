#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//所持防具のクラス。
class HoldArmor :public HoldItemBase
{
public:
	HoldArmor(Item::BaseInfo* info);
	~HoldArmor();

	//防具の独自パラメーターを設定。
	//引数：防御力、魔法防御力。
	inline void SetParam(float Def, float MDef) {
		_Def += _Def*Def;
		_MagicDef += _MagicDef*MDef;
	}

	//防御力の乱数差分を取得。
	inline int GetDefRnd() {
		return _DefRnd;
	}

	//魔法防御力の乱数差分を取得。
	inline int GetMDefRnd() {
		return _MDefRnd;
	}
private:
	int _DefRnd;	//防御力の乱数差分(この値でランク付け、単位はパーセント)。
	int _MDefRnd;	//魔法防御力の乱数差分(この値でランク付け、単位はパーセント)。
	int _Def;		//ランクを考慮した物理防御力。
	int _MagicDef;	//ランクを考慮した魔法防御力。
};