#pragma once
#include "HoldEquipment.h"

//所持している武器のクラス。
class HoldWeapon :public HoldEquipment
{
public:
	HoldWeapon(char* name);
	~HoldWeapon();


	//武器のパラメーターをランダムで算出。
	void CreateRandParam()override;

	//武器のパラメーターを基準値で設定。
	void CreateOriginParam() override;

	//攻撃力の乱数差分を取得。
	inline int GetAtkRnd() {
		return _AtkRnd;
	}

	//魔法攻撃力の乱数差分を取得。
	inline int GetMtkRnd() {
		return _MAtkRnd;
	}

	//クリティカル率の乱数差分を取得。
	inline int GetCrtRnd() {
		return _DexRnd;
	}

	//ランクを考慮した物理攻撃力(クリティカル率は除く)を取得。
	inline int GetAtk() {
		return _Atk;
	}

	//ランクを考慮した魔法攻撃力クリティカル率は除く)を取得。
	inline int GetMagicAtk() {
		return _MagicAtk;
	}

	//ランクを考慮したクリティカル率を取得。
	inline int GetDex() {
		return _Dex;
	}

	//武器の基準値と差分値の割合を算出。
	inline float ParamRaitoMass()override {
		float offset = _AtkRnd + _MAtkRnd + _DexRnd;
		float sum = _Atk + _MagicAtk + _Dex;
		float par = offset / sum;
		return par;
	}

	//ランクを考慮したランダム物理攻撃力の計算。
	inline void RndAtkMass() {
		//物理攻撃力のランダム差分算出。
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Atk;
		int rnd = GetRand_S50to100();// -50から100の値をランダムで取得。
		float raito = static_cast<float>(rnd) * 0.01f;

		//最終的な物理攻撃力を算出。
		_AtkRnd = baseParam * raito;
		_Atk = baseParam + _AtkRnd;
	}

	//ランクを考慮したランダム魔法攻撃力の計算。
	inline void RndMAtkMass() {
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;
		int rnd = GetRand_S50to100();	// -50から100の値をランダムで取得。
		float raito = static_cast<float>(rnd) * 0.01f;

		//最終的な魔法攻撃力を算出。
		_MAtkRnd = baseParam * raito;
		_MagicAtk = baseParam + _MAtkRnd;
	}

	//ランクを考慮したランダムクリティカル率の計算。
	inline void RndCrtMass() {
		//最終的なクリティカルのランダム差分算出。
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Dex;
		int rnd = GetRand_S50to100();
		float raito = static_cast<float>(rnd) * 0.01f;

		//最終的なクリティカル率を算出。
		_DexRnd = baseParam * raito;
		_Dex = baseParam + _DexRnd;
	}



private:

	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// ※CSVから読み込んだランダムパラメータ情報や装備情報を使用する際はこの関数でパラメータを設定する。
	void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info)override;

private:
	int _AtkRnd;		//攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	int _MAtkRnd;		//魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	int _DexRnd;		//クリティカル率の乱数差分(この値でランク付け、単位はパーセント)。
	int	_Atk;			//ランクを考慮した物理攻撃力。
	int	_MagicAtk;		//ランクを考慮した魔法攻撃力。
	int _Dex;			//ランクを考慮したクリティカル率。
};