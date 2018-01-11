#pragma once
#include "HoldEquipment.h"

//所持している武器のクラス。
class HoldWeapon :public HoldEquipment
{
private:
	friend class HoldItemFactory;
	HoldWeapon(char* name);
public:
	~HoldWeapon();


	//武器のパラメーターをランダムで算出。
	void CreateRandParam()override;

	//武器のパラメーターを基準値で設定。
	void CreateOriginParam() override;

	//攻撃力の乱数差分を取得。
	inline int GetAtkRnd() const {
		return _AtkRnd;
	}

	//魔法攻撃力の乱数差分を取得。
	inline int GetMtkRnd() const{
		return _MAtkRnd;
	}

	//クリティカル率の乱数差分を取得。
	inline int GetDexRnd() const{
		return _DexRnd;
	}

	//ランクを考慮した物理攻撃力(クリティカル率は除く)を取得。
	inline int GetAtk() const{
		return _Atk;
	}

	//ランクを考慮した魔法攻撃力クリティカル率は除く)を取得。
	inline int GetMagicAtk()const {
		return _MagicAtk;
	}

	//ランクを考慮したクリティカル率を取得。
	inline int GetDex()const {
		return _Dex;
	}

	// クリティカル威力取得。
	inline int GetCrt()const {
		return _Crt;
	}

	////武器の基準値と差分値の割合を算出。
	//inline float SumRaitoMass()override {
	//	float offset = static_cast<float>(_AtkRnd + _MAtkRnd + _DexRnd);
	//	float sum = static_cast<float>(_Atk + _MagicAtk + _Dex);
	//	float par = offset / (sum - offset);
	//	return par;
	//}

	//ランクを考慮したランダム物理攻撃力の計算。
	// 引数：	差分率。
	inline void RndAtkMass(float offset) {
		//物理攻撃力のランダム差分算出。
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Atk;

		//最終的な物理攻撃力を算出。
		_AtkRnd = static_cast<int>(baseParam * offset);
		_Atk = baseParam + _AtkRnd;
	}

	//ランクを考慮したランダム魔法攻撃力の計算。
	// 引数：	差分率。
	inline void RndMAtkMass(float offset) {
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;

		//最終的な魔法攻撃力を算出。
		_MAtkRnd = static_cast<int>(baseParam * offset);
		_MagicAtk = baseParam + _MAtkRnd;
	}

	//ランクを考慮したランダムクリティカル率の計算。
	// 引数：	差分率。
	inline void RndDexMass(float offset) {
		//最終的なクリティカルのランダム差分算出。
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Dex;

		//最終的なクリティカル率を算出。
		_DexRnd = static_cast<int>(baseParam * offset);
		_Dex = baseParam + _DexRnd;
	}



private:

	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// ※CSVから読み込んだランダムパラメータ情報や装備情報を使用する際はこの関数でパラメータを設定する。
	void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info)override;

private:
	int _AtkRnd;		//攻撃力の乱数差分(この値でランク付け)。
	int _MAtkRnd;		//魔法攻撃力の乱数差分(この値でランク付け)。
	int _DexRnd;		//クリティカル率の乱数差分(この値でランク付け)。
	int	_Atk;			//ランクを考慮した物理攻撃力。
	int	_MagicAtk;		//ランクを考慮した魔法攻撃力。
	int _Dex;			//ランクを考慮したクリティカル率。
	int _Crt;			//クリティカル威力。
};