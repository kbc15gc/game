#pragma once
#include "HoldEquipment.h"

//所持している武器のクラス。
class HoldWeapon :public HoldEquipment
{
public:
	HoldWeapon(char* name);
	~HoldWeapon();

	//武器の独自パラメーターを設定。
	//引数：攻撃力の乱数差分(この値でランク付け、単位はパーセント)、魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	inline void SetParam(float AtkRnd, float MAtkRnd, float CrtRnd) {
		_AtkRnd = AtkRnd;
		_MAtkRnd = MAtkRnd;
		_Atk = _Atk + AtkRnd;
		_MagicAtk = _MagicAtk + MAtkRnd;
		_Crt = _Crt + CrtRnd;
	}
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
		return _CrtRnd;
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
	inline int GetCrt() {
		return _Crt;
	}

	//武器のランクを算出。
	inline float ParamRaitoMass()override {
		float offset = _AtkRnd + _MAtkRnd + _CrtRnd;
		float sum = _Atk + _MagicAtk + _Crt;
		float par = offset / sum;
		return par;
	}

	//ランクを考慮したランダム物理攻撃力の計算。
	inline void RndAtkMass() {
		//物理攻撃力のランダム差分算出。
		int baseParam = _Atk;
		int rnd = GetRand_S50to100();// -50から100の値をランダムで取得。
		float raito = static_cast<float>(rnd) * 0.01f;

		//最終的な物理攻撃力を算出。
		_AtkRnd = baseParam * raito;
		_Atk = baseParam + _AtkRnd;
	}

	//ランクを考慮したランダム魔法攻撃力の計算。
	inline void RndMAtkMass() {
		int baseParam = _MagicAtk;
		int rnd = GetRand_S50to100();	// -50から100の値をランダムで取得。
		float raito = static_cast<float>(rnd) * 0.01f;

		//最終的な魔法攻撃力を算出。
		_MAtkRnd = baseParam * raito;
		_MagicAtk = baseParam + _MAtkRnd;
	}

	//ランクを考慮したランダムクリティカル率の計算。
	inline void RndCrtMass() {
		//最終的なクリティカルのランダム差分算出。
		int baseParam = _Crt;
		int rnd = GetRand_S50to100();
		float raito = static_cast<float>(rnd) * 0.01f;
		_CrtRnd = baseParam * raito;

		//最終的なクリティカル率を算出。
		_Crt = baseParam + _CrtRnd;
	}
private:
	int _AtkRnd;		//攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	int _MAtkRnd;		//魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	int _CrtRnd;		//クリティカル率の乱数差分(この値でランク付け、単位はパーセント)。
	int	_Atk;			//ランクを考慮した物理攻撃力。
	int	_MagicAtk;		//ランクを考慮した魔法攻撃力。
	int _Crt;			//ランクを考慮したクリティカル率。
};