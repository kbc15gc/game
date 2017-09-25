#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//装備品の基底クラス。
class HoldEquipment :public HoldItemBase
{
public:
	//コンストラクタ。
	HoldEquipment(Item::BaseInfo* info);

	//デストラクタ。
	~HoldEquipment();

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

	//武器もしくは防具のランクを算出。
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

	//装備フラグをセット(tureなら装備している、falseなら装備していない)。
	inline bool GetIsEquip() {
		return _IsEquip;
	}

protected:
	Rank _Rank;					//装備品のランク。
	bool _IsEquip = false;		//装備されているかのフラグ(tureなら装備されている)。
};

