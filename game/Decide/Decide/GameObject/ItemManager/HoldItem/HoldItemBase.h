#pragma once
#include "GameObject\ItemManager\ItemManager.h"

namespace {
	//アイテムごとの所持数(書き出しと読み込みに使用)。
	struct HoldInfo
	{
		HoldInfo() {

		}
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			所持数。
		HoldInfo(int TypeID,int ID,int HoldNum) {
			_TypeID = TypeID;
			_ID = ID;
			_HoldNum = HoldNum;
		}
		int _TypeID;		//アイテムの種類(消費アイテムか武器か防具か)。
		int _ID;			//アイテムの通し番号(TypeIDの中でユニーク)。
		int _HoldNum;		//所持数。
	};

	static Support::DATARECORD HoldItemData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldInfo,_HoldNum),		sizeof(int) },
	};

	struct HoldWeponInfo : public HoldInfo {
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			所持数。
		//			攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		//			魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		HoldWeponInfo(int TypeID, int ID, int HoldNum, int AtkRnd, int MAtkRnd) : HoldInfo(TypeID, ID, HoldNum) {
			_AtkRnd = AtkRnd;
			_MAtkRnd = MAtkRnd;
		}
		int _AtkRnd;		//攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		int _MAtkRnd;		//魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	};

	static Support::DATARECORD HoldWeaponData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldWeponInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_HoldNum),		sizeof(int) },
		{ "AtkRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_AtkRnd),		sizeof(int) },
		{ "MagicRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_MAtkRnd),		sizeof(int) },
	};

	struct HoldArmorInfo : public HoldInfo {
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			所持数。
		//			防御力のランク差分。
		//			魔法防御力のランク差分。
		HoldArmorInfo(int TypeID, int ID, int HoldNum, int Def, int MDef) : HoldInfo(TypeID, ID, HoldNum) {
			_DefRnd = Def;
			_MDefRnd = MDef;
		}
		int _DefRnd;	//防御力のランク差分。
		int _MDefRnd;	//魔法防御力のランク差分。
	};

	static Support::DATARECORD HoldArmorData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldArmorInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_HoldNum),		sizeof(int) },
		{ "DefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_DefRnd),		sizeof(int) },
		{ "MDefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_MDefRnd),		sizeof(int) },
	};
}

//所持アイテムの基底クラス。
class HoldItemBase
{
public:
	// 引数：	アイテムの情報構造体。
	HoldItemBase(Item::BaseInfo* info);
	virtual ~HoldItemBase();

	// コメントの数字は基準値からの加算率の幅。
	enum Rank
	{
		SS = 0, //91 ~ 100	神造級。
		S, //71 ~ 90		宝物級。
		A,	// 51 ~ 70		高級。
		B,	// 31 ~ 50		良。
		C,	// -20 ~ 30		平均。
		D,	// -40 ~ -21	粗悪。
		E,	// -50 ~ -41	超粗悪。
	};

	//構造体の情報を取得。
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//所持数を更新(減らす場合は引数にマイナスを設定)。
	inline void UpdateHoldNum(int add = 1) {
		_HoldNum += add;
	}

	//武器もしくは防具のランクを算出。
	virtual inline float ParamRaitoMass() {
		return 0.0f;
	}

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

	//所持数を取得。
	inline int GetHoldNum() {
		return  _HoldNum;
	}

	//-50から100の値をランダムで取得。
	inline float GetRand_S50to100() {
		return (rand() % 151) - 50;
	}

private:
	int _HoldNum = 0;
	Item::BaseInfo* _Info;
protected:
	Rank _Rank;
};