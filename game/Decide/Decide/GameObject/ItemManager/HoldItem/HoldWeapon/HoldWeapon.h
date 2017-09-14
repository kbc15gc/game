#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//所持している武器のクラス。
class HoldWeapon :public HoldItemBase
{
public:
	HoldWeapon(Item::BaseInfo* info);
	~HoldWeapon();

	//武器の独自パラメーターを設定。
	//引数：攻撃力の乱数差分(この値でランク付け、単位はパーセント)、魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	inline void SetParam(float AtkRnd, float MAtkRnd) {
		_AtkRnd = AtkRnd;
		_MAtkRnd = MAtkRnd;
		_Atk += _Atk * AtkRnd;
		_MagicAtk += _MagicAtk*MAtkRnd;
	}

	//攻撃力の乱数差分を取得。
	inline int GetAtkRnd() {
		return _AtkRnd;
	}

	//魔法攻撃力の乱数差分を取得。
	inline int GetMtkRnd() {
		return _MAtkRnd;
	}

	//ランクを考慮した物理攻撃力(クリティカル率は除く)を取得。
	inline int GetAtk() {
		return _Atk;
	}

	//ランクを考慮した魔法攻撃力クリティカル率は除く)を取得。
	inline int GetMagicAtk() {
		return _MagicAtk;
	}

private:
	int _AtkRnd;		//攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	int _MAtkRnd;		//魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	int	_Atk;			//ランクを考慮した物理攻撃力。
	int	_MagicAtk;		//ランクを考慮した魔法攻撃力。
};