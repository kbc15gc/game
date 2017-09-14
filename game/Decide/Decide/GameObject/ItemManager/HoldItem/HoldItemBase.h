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

	enum Rank
	{
		S = 0,
		A,
		B,
		C,
		D,
		E,
	};

	//構造体に情報を設定。
	inline void SetInfo(Item::BaseInfo* info) {
		_Info = info;
	}

	//構造体の情報を取得。
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//所持数を更新(減らす場合は引数にマイナスを設定)。
	inline void UpdateHoldNum(int add = 1) {
		_HoldNum += add;
	}

	//所持数を取得。
	inline int GetHoldNum() {
		return  _HoldNum;
	}

private:
	int _HoldNum = 0;
	Item::BaseInfo* _Info;
protected:
	Rank _Rank;
};