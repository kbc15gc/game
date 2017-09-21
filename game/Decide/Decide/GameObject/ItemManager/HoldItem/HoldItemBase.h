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
		HoldWeponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd,int CrtRnd) : HoldInfo(TypeID, ID, 0) {
			_AtkRnd = AtkRnd;
			_MAtkRnd = MAtkRnd;
			_CrtRnd = CrtRnd;
		}
		int _AtkRnd;		//攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		int _MAtkRnd;		//魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		int _CrtRnd;		//クリティカル率の乱数差分(この値でランク付け、単位はパーセント)。
	};

	static Support::DATARECORD HoldWeaponData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldWeponInfo,_ID),			sizeof(int) },
		{ "AtkRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_AtkRnd),		sizeof(int) },
		{ "MagicRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_MAtkRnd),		sizeof(int) },
		{ "CrtRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_CrtRnd),		sizeof(int) },
	};

	struct HoldArmorInfo : public HoldInfo {
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			所持数。
		//			防御力のランク差分。
		//			魔法防御力のランク差分。
		HoldArmorInfo(int TypeID, int ID, int Def, int MDef) : HoldInfo(TypeID, ID, 0) {
			_DefRnd = Def;
			_MDefRnd = MDef;
		}
		int _DefRnd;	//防御力のランク差分。
		int _MDefRnd;	//魔法防御力のランク差分。
	};

	static Support::DATARECORD HoldArmorData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldArmorInfo,_ID),			sizeof(int) },
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

	//構造体の情報を取得。
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//-50から100の値をランダムで取得。
	inline float GetRand_S50to100() {
		return (rand() % 151) - 50;
	}
protected:
	Item::BaseInfo* _Info;
};