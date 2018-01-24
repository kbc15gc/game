#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"



//装備品の基底クラス。
class HoldEquipment :public HoldItemBase
{
public:
	// コメントの数字は基準値からの加算率の幅。
	//@todo for debug
	//後で調整してね。
	enum Rank
	{
		SS = 0, // 神造級。
		S,		// 宝物級。
		A,		// 高級。
		B,		// 良。
		C,		// 平均。
		None,	// 装備無し.
		Max,	// 数.
	};

protected:
	friend class HoldItemFactory;
	HoldEquipment(char* name);
public:
	~HoldEquipment();


	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// ※CSVから読み込んだランダムパラメータ情報や装備情報を使用する際はこの関数でパラメータを設定する。
	virtual void ConfigLoadData(Hold::HoldInfo* info);

	//// 補正値から装備品のランクを決定する。
	//// 引数：	補正値。
	//inline void RankSelect(float ratio) {
	//	if (ratio >= 0.26f && ratio <= 0.5f) {
	//		// 26% ～ 50%。
	//		_Rank = Rank::SS;
	//	}
	//	else if (ratio >= 0.16f && ratio <= 0.25f) {
	//		// 16% ～ 25%。
	//		_Rank = Rank::S;
	//	}
	//	else if (ratio >= 0.11f && ratio <= 0.15f) {
	//		// 11% ～ 15%。
	//		_Rank = Rank::A;
	//	}
	//	else if (ratio >= 0.06f && ratio <= 0.1f) {
	//		// 6% ～ 10%。
	//		_Rank = Rank::B;
	//	}
	//	else if (ratio >= -0.19f && ratio <= 0.05f) {
	//		// -19% ～ 5%。
	//		_Rank = Rank::C;
	//	}
	//	else if (ratio >= -0.34f && ratio <= -0.2f) {
	//		// -34% ～ -20%。
	//		_Rank = Rank::D;
	//	}
	//	else if (ratio >= -0.5f && ratio <= -0.35f) {
	//		// -50% ～ -35%。
	//		_Rank = Rank::E;
	//	}
	//	else {
	//		_Rank = Rank::None;
	//	}

	//	//補正値格納。
	//	_Revision = ratio;
	//}

	//武器または防具のランクを決定し、補正値を算出する。
	inline void RankSelect() {
		int rnd = rand() % 10000;	// 0.01%～100.00%の確率を計算。

		if (rnd >= 0 && rnd <= 999) {
			// 10.00%。
			_Rank = Rank::SS;
			_Revision = ParamRaitoMass(26, 50);
		}
		else if (rnd >= 1000 && rnd <= 2999) {
			// 20.00%。
			_Rank = Rank::S;
			_Revision = ParamRaitoMass(16,25);
		}
		else if (rnd >= 3000 && rnd <= 7999) {
			// 50.00%。
			_Rank = Rank::A;
			_Revision = ParamRaitoMass(11, 15);
		}
		else if (rnd >= 8000 && rnd <= 9999) {
			// 20.00%。
			_Rank = Rank::B;
			_Revision = ParamRaitoMass(6, 10);
		}
	}

	//引数１から引数２の間で装備の差分値の割合を算出。
	// 引数：	下限(%)。
	//			上限(%)。
	inline float ParamRaitoMass(int min,int max) {
		int rnd;
		rnd = rand() % ((max - min) + 1) + min;
		return static_cast<float>(rnd) * 0.01f;
	}

	virtual inline float SumRaitoMass() { return 0.0f; };

	//武器、防具のパラメーターをランダムで算出。
	virtual void CreateRandParam() {
		
	}

	//武器、防具のパラメーターを基準値で設定。
	virtual void CreateOriginParam() {

	}

	//装備するならこの関数を呼ぶ。
	inline void SetIsEquipTrue() {
		_IsEquip = true;
	}

	//装備しないならこの関数を呼ぶ。
	inline void SetIsEquipFalse() {
		_IsEquip = false;
	}

	//装備フラグを取得(tureなら装備している、falseなら装備していない)。
	inline bool GetIsEquip() {
		return _IsEquip;
	}

	Rank GetRank()
	{
		return _Rank;
	}

	//補正値を取得。
	float GetRevision()
	{
		return _Revision;
	}

	//値段取得。
	/*int GetValue()override
	{
		return  _Info->Value + _Info->Value * GetRevision();
	}*/
private:
	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// 継承先によって処理を変更。
	virtual void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) = 0;

protected:
	Rank _Rank;	//装備品のランク。
	
	float _Revision = 0.0f;		//補正値。
	bool _IsEquip = false;		//装備されているかのフラグ(tureなら装備されている)。

	bool _isLoad = false;	// CSVから読み込んだデータか。
};

static char* RankText[HoldEquipment::Rank::Max] =
{
	"SS",
	"S",
	"A",
	"B",
	"C",
	"None",
};