﻿#pragma once
#include "GameObject\ItemManager\ItemManager.h"

class SoundSource;
class HoldItemBase;
class HoldEquipment;
class HoldWeapon;
class HoldArmor;
class ConsumptionItem;

const int INVENTORYLISTNUM = 20;


namespace {
	//アイテムごとの所持数(書き出しと読み込みに使用)。
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

	struct ConsumptionInfo :public HoldInfo
	{
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

	struct HoldWeponInfo : public HoldInfo {
		// 引数：	アイテム種別。
		//			アイテム通し番号。
		//			攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		//			魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
		HoldWeponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd, int CrtRnd);
		// 引数：	コピー元のポインタ。
		HoldWeponInfo(HoldItemBase* info);

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
		HoldArmorInfo(int TypeID, int ID, int Def, int MDef);
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
	};
};


//インベントリクラス。
class Inventory
{
private:
	Inventory();

public:

	void Initialize();

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->Initialize();
		}
		return _InventoryInstance;
	}


	//プレイヤーの所持金を取得。
	int GetPlayerMoney() {
		return _PlayerMoney;
	}

	int* GetPlayerMoneyPt()
	{
		return &_PlayerMoney;
	}

	//プレイヤーの所持金に加算。
	void AddPlayerMoney(int add)
	{
		_PlayerMoney += add;
	}

	//プレイヤーの所持金から減算。
	void SubtractPlayerMoney(int sub) {
		_PlayerMoney -= sub;
	}

	//アイテムをインベントリに追加(第一引数追加するアイテムの種類、第二引数がアイテムの情報、追加する数(指定しない場合は1)。)。
	void AddItem(Item::BaseInfo* item, int num = 1);

	//指定されたインベントリのリストの先頭を取得。
	 inline const vector<HoldItemBase*>& GetInventoryList( Item::ItemCodeE code) {
		return _InventoryItemList[static_cast<int>(code)];
	}

	//アイテムコードとIDを元に配列から検索。
	HoldItemBase* FindItem( Item::ItemCodeE code, const unsigned int& id);

	//アイテムを使う。
	void UseItem();

	//所持数を減らす。
	bool SubHoldNum(Item::BaseInfo* item,int num);

	//第一引数：追加したい装備のポインタ、第二引数:武器か防具のアイテムコード(アイテムは無効)。
	void Inventory::AddEquipment(HoldEquipment* equi, Item::ItemCodeE code);

private:
	//リストから指定されたアイテムを削除。
	void _DeleteFromList(HoldItemBase* item);

	//アイテムリストのデータを保存。
	void _ItemListOutData();
private:
	
	//インベントリ。
	vector<vector<HoldItemBase*>> _InventoryItemList;

	//今見ているアイテム。
	int _NowLookItemPos = -1;

	//所持金。
	int _PlayerMoney = 100;

	static Inventory* _InventoryInstance;
};
