#pragma once
#include "HoldEquipment.h"

//所持防具のクラス。
class HoldArmor :public HoldEquipment
{
public:
	HoldArmor(Item::BaseInfo* info);
	~HoldArmor();

	//防具のパラメーターをランダムで算出。
	void CreateRandParam()override;

	//防具のパラメーターを基準値で設定。
	void CreateOriginParam() override;

	//防御力の乱数差分を取得。
	inline int GetDefRnd() {
		return _DefRnd;
	}

	//魔法防御力の乱数差分を取得。
	inline int GetMDefRnd() {
		return _MDefRnd;
	}

	//ランクを考慮した物理防御力を取得。
	inline int GetDef() {
		return _Def;
	}

	//ランクを考慮した魔法防御力を取得。
	inline int GetMagicDef() {
		return _MagicDef;
	}

	//防具のランクを算出。
	inline float ParamRaitoMass()override {
		float offset = _DefRnd + _MDefRnd;
		float sum = _Def + _MagicDef;
		float par = offset / sum;
		return par;
	}

	//ランクを考慮した物理防御力を計算。
	inline void RndDefMass() {
		// 物理防御力のランダム差分算出。
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->Def;
		int rnd = GetRand_S50to100();// -50から100の値をランダムで取得。
		float raito = static_cast<float>(rnd) * 0.01f;

		//最終的な物理防御力を算出。
		_DefRnd = baseParam * raito;
		_Def = baseParam + _DefRnd;
	}

	//ランクを考慮した魔法防御力を計算。
	inline void RndMDef() {
		// 魔法防御力のランダム差分算出。
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;
		int rnd = GetRand_S50to100();	// -50から100の値をランダムで取得。
		float raito = static_cast<float>(rnd) * 0.01f;

		//最終的な魔法防御力。
		_MDefRnd = baseParam * raito;
		_MagicDef = baseParam + _MDefRnd;
	}
private:
	int _DefRnd;	//防御力の乱数差分(この値でランク付け、単位はパーセント)。
	int _MDefRnd;	//魔法防御力の乱数差分(この値でランク付け、単位はパーセント)。
	int _Def;		//ランクを考慮した物理防御力。
	int _MagicDef;	//ランクを考慮した魔法防御力。
};