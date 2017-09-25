#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//装備品の基底クラス。
class HoldEquipment :public HoldItemBase
{
public:
	// コメントの数字は基準値からの加算率の幅。
	enum Rank
	{
		SS = 0, //91 ~ 100	神造級。
		S,		//71 ~ 90	宝物級。
		A,		//51 ~ 70	高級。
		B,		//31 ~ 50	良。
		C,		//-20 ~ 30	平均。
		D,		//-40 ~ -21	粗悪。
		E,		//-50 ~ -41	超粗悪。
	};

public:
	HoldEquipment(char* name);
	~HoldEquipment();


	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// ※CSVから読み込んだランダムパラメータ情報や装備情報を使用する際はこの関数でパラメータを設定する。
	void ConfigLoadData(HoldEquipInfo* info);

	//武器または防具のランクを決定。
	inline void RankSelect(float raito) {
		if (raito >= -0.5f && raito <= -0.41f) {
			_Rank = Rank::E;
		}
		else if (raito >= -0.4f && raito <= -0.21f) {
			_Rank = Rank::D;
		}
		else if (raito >= -0.2f && raito <= 0.3f) {
			_Rank = Rank::C;
		}
		else if (raito >= 0.31f && raito <= 0.5f) {
			_Rank = Rank::B;
		}
		else if (raito >= 0.51f && raito <= 0.7f) {
			_Rank = Rank::A;
		}
		else if (raito >= 0.71f && raito <= 0.9f) {
			_Rank = Rank::S;
		}
		else if (raito >= 0.9f && raito <= 1.0f) {
			_Rank = Rank::SS;
		}

	}

	//装備の基準値と差分値の割合を算出。
	virtual inline float ParamRaitoMass() {
		return 0.0f;
	}

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


private:
	// 外部から読み込んだデータを設定。
	// 引数：	CSV読み書き用の所持装備品構造体へのポインタ。
	// 継承先によって処理を変更。
	virtual void _ConfigLoadDataSubClass(HoldEquipInfo* info) = 0;

protected:
	Rank _Rank;	//装備品のランク。
	bool _IsEquip = false;		//装備されているかのフラグ(tureなら装備されている)。

	bool _isLoad = false;	// CSVから読み込んだデータか。
};

