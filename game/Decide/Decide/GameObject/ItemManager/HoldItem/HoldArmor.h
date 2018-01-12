#pragma once
#include "HoldEquipment.h"

//所持防具のクラス。
class HoldArmor :public HoldEquipment
{
private:
	friend class HoldItemFactory;
	HoldArmor(char* name);
public:
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

	////防具の基準値と差分値の割合を算出。
	//inline float SumRaitoMass()override {
	//	float offset = static_cast<float>(_DefRnd + _MDefRnd);
	//	float sum = static_cast<float>(static_cast<Item::ArmorInfo*>(_Info)->Def + static_cast<Item::ArmorInfo*>(_Info)->MagicDef);
	//	float par = offset / sum;
	//	return par;
	//}

	//ランクを考慮した物理防御力を計算。
	// 引数：	差分率。
	inline void RndDefMass(float offset) {
		// 物理防御力のランダム差分算出。
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->Def;

		//最終的な物理防御力を算出。
		_DefRnd = static_cast<int>(baseParam * offset);
		_Def = baseParam + _DefRnd;
	}

	//ランクを考慮した魔法防御力を計算。
	// 引数：	差分率。
	inline void RndMDef(float offset) {
		// 魔法防御力のランダム差分算出。
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;
		//最終的な魔法防御力。
		_MDefRnd = static_cast<int>(baseParam * offset);
		_MagicDef = baseParam + _MDefRnd;
	}

private:
	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// ※CSVから読み込んだランダムパラメータ情報や装備情報を使用する際はこの関数でパラメータを設定する。
	void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info)override;

private:
	int _DefRnd = 0;	//防御力の乱数差分(この値でランク付け、単位はパーセント)。
	int _MDefRnd = 0;	//魔法防御力の乱数差分(この値でランク付け、単位はパーセント)。
	int _Def;		//ランクを考慮した物理防御力。
	int _MagicDef;	//ランクを考慮した魔法防御力。
};