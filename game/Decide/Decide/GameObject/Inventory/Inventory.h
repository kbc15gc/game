#pragma once
#include "GameObject\ItemManager\ItemManager.h"

class SoundSource;
class HoldItemBase;
class HoldEquipment;
class HoldWeapon;
class HoldArmor;
class ConsumptionItem;

const int INVENTORYLISTNUM = 20;


namespace Hold{
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
		HoldEquipInfo(int TypeID, int ID,bool isEquip);

		// 引数：	コピー元のポインタ。
		HoldEquipInfo(HoldItemBase* info);

		int _IsEquip;		//装備されているかフラグ。(tureなら装備されている。falseなら装備してない)。
	};

	// 武器の所持情報。
	struct HoldWeaponInfo : public HoldEquipInfo {
		HoldWeaponInfo(){};

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
		int _CrtRnd;		//クリティカル率の乱数差分(この値でランク付け、単位はパーセント)。
	};

	static Support::DATARECORD HoldWeaponData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldWeaponInfo,_ID),			sizeof(int) },
		{ "AtkRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_AtkRnd),		sizeof(int) },
		{ "MagicRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_MAtkRnd),		sizeof(int) },
		{ "CrtRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_CrtRnd),		sizeof(int) },
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
		int _IsEquip;	//装備されているかフラグ。(tureなら装備されている。falseなら装備してない)
	};

	static Support::DATARECORD HoldArmorData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldArmorInfo,_ID),			sizeof(int) },
		{ "DefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_DefRnd),		sizeof(int) },
		{ "MDefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_MDefRnd),		sizeof(int) },
		{ "IsEquip",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_IsEquip),		sizeof(int) },
	};
};


//インベントリクラス。
class Inventory
{
private:
	Inventory();
	virtual ~Inventory(); 
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

	//アイテムをインベントリに追加(第一引数追加するアイテムの情報、追加する数(指定しない場合は1)。)。
	void AddItem(Item::ItemInfo* item, int num = 1);

	// 装備品追加関数
	// 引数：	追加したい装備品の情報構造体へのポインタ。
	//			ランダムパラメータにするか。
	// 戻り値：	追加した装備品のポインタ。
	HoldEquipment* AddEquipment(Item::BaseInfo* info, bool isRandParam);

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

	//インベントリ内を整理(途中にnullを挟まない用に中身を詰めるだけ)。
	void ArrangementInventory();

	//インベントリを整列(ID順になる)。
	void SortID();

	// 指定したアイテムの合計所持数を返却。
	inline int GetHoldNum(Item::ItemCodeE code,int id) {
		return _HoldNumList[static_cast<int>(code)][id];
	}

private:

	//リストから指定されたアイテムを削除。
	void _DeleteFromList(HoldItemBase* item);

	// 所持品のデータを読み込み。
	void _LoadData();

	//所持品のデータを書き出し。
	// 引数：	アイテムコード。
	void _OutData(Item::ItemCodeE code);

	// 全所持品書き出し。
	void _OutData_All();


	//ID比較関数。
	//bool IDComp(HoldItemBase* left, HoldItemBase* rigth);


private:
	
	//インベントリ。
	vector<vector<HoldItemBase*>> _InventoryItemList;

	//今見ているアイテム。
	int _NowLookItemPos = -1;

	//所持金。
	int _PlayerMoney = 100;

	GameObject* _Player;

	// アイテムの種類ごとに所持数をまとめた配列。
	// 添え字：	Item::ItemCodeE。
	//			ID。
	vector<vector<int>> _HoldNumList;

	static Inventory* _InventoryInstance;
};
