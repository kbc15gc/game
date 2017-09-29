#pragma once
#include "GameObject\ItemManager\ItemManager.h"

class HoldItemBase;

namespace Hold {
	//アイテムの所持情報(書き出しと読み込みに使用)。
	struct HoldInfo
	{
		HoldInfo();
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		HoldInfo(int TypeID, int ID);
		// 引数：	コピー元のポインタ。
		HoldInfo(HoldItemBase* info);

		int _TypeID;		//アイテムの種類(消費アイテムか武器か防具か)。
		int _ID;			//アイテムの通し番号(TypeIDの中でユニーク)。
	};

	// 消費アイテムの所持情報。
	struct ConsumptionInfo :public HoldInfo
	{
		ConsumptionInfo() {};
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			所持数。
		ConsumptionInfo(int TypeID, int ID, int num);

		// 引数：	コピー元のポインタ。
		ConsumptionInfo(HoldItemBase* info);

		int _HoldNum;		//所持数。
	};

	static Support::DATARECORD ConsumptionItemData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct ConsumptionInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ConsumptionInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct ConsumptionInfo,_HoldNum),		sizeof(int) },
	};

	// 装備アイテムの所持情報。
	struct HoldEquipInfo : public HoldInfo {
		HoldEquipInfo();

		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			装備されているか。
		HoldEquipInfo(int TypeID, int ID, bool isEquip);

		// 引数：	コピー元のポインタ。
		HoldEquipInfo(HoldItemBase* info);

		int _IsEquip;		//装備されているかフラグ。(tureなら装備されている。falseなら装備してない)。
	};

	// 武器の所持情報。
	struct HoldWeaponInfo : public HoldEquipInfo {
		HoldWeaponInfo() {};

		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		//			魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		//			装備されているか。
		HoldWeaponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd, int CrtRnd, bool IsEquip);
		// 引数：	コピー元のポインタ。
		HoldWeaponInfo(HoldItemBase* info);

		int _AtkRnd;		//攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		int _MAtkRnd;		//魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		int _DexRnd;		//クリティカル率の乱数差分(この値でランク付け、単位はパーセント)。
	};

	static Support::DATARECORD HoldWeaponData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldWeaponInfo,_ID),			sizeof(int) },
		{ "AtkRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_AtkRnd),		sizeof(int) },
		{ "MagicRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_MAtkRnd),		sizeof(int) },
		{ "DexRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_DexRnd),		sizeof(int) },
		{ "IsEquip",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_IsEquip),		sizeof(int) },
	};

	// 防具の所持情報。
	struct HoldArmorInfo : public HoldEquipInfo {
		HoldArmorInfo() {};
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			所持数。
		//			防御力のランク差分。
		//			魔法防御力のランク差分。
		//			装備されているか。
		HoldArmorInfo(int TypeID, int ID, int Def, int MDef, bool IsEquip);
		// 引数：	コピー元のポインタ。
		HoldArmorInfo(HoldItemBase* info);

		int _DefRnd;	//防御力のランク差分。
		int _MDefRnd;	//魔法防御力のランク差分。
	};

	static Support::DATARECORD HoldArmorData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldArmorInfo,_ID),			sizeof(int) },
		{ "DefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_DefRnd),		sizeof(int) },
		{ "MDefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_MDefRnd),		sizeof(int) },
		{ "IsEquip",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_IsEquip),		sizeof(int) },
	};
};


class HoldItemFactory {
public:
	// 所持品作成関数。
	// 引数：	アイテムの情報ポインタ。
	//			ランダムパラメータを生成するか。
	static HoldItemBase* CreateItem(Item::BaseInfo* info, bool isRandParam);

	// 所持品作成関数。
	// 引数：	アイテム保存情報ポインタ。
	static HoldItemBase* CreateItem(Hold::HoldInfo* info);

	// 所持品作成関数。
	// 引数：	アイテムの種類。
	//			アイテムの番号。
	//			ランダムパラメータを生成するか。
	static HoldItemBase* CreateItem(Item::ItemCodeE code, int id, bool isRandParam);
};