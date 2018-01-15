#pragma once
#include "GameObject\ItemManager\ItemManager.h"
class SoundSource;
class HoldItemBase;
class HoldEquipment;
class HoldWeapon;
class HoldArmor;
class ConsumptionItem;

class ImageObject;
class TextObject;

const int INVENTORYLISTNUM = 40;



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
		}
		return _InventoryInstance;
	}


	//プレイヤーの所持金を取得。
	int GetPlayerMoney() {
		SaveMoney();
		return _PlayerMoney;
	}

	int* GetPlayerMoneyPt()
	{
		SaveMoney();
		return &_PlayerMoney;
	}

	//プレイヤーの所持金に加算。
	void AddPlayerMoney(int add)
	{
		_PlayerMoney += add;
		SaveMoney();
	}

	//プレイヤーの所持金から減算。
	void SubtractPlayerMoney(int sub) {
		_PlayerMoney -= sub;
		SaveMoney();
	}

	//アイテムをインベントリに追加。
	// 引数：	アイテムの情報のポインタ。
	//			追加する数(指定しない場合は1)。
	// 戻り値：	追加成功か。
	bool AddItem(Item::ItemInfo* item, int num = 1);

	// 装備品追加関数
	// 引数：	追加したい装備品の情報構造体へのポインタ。
	//			ランダムパラメータにするか。
	// 戻り値：	追加した装備品のポインタ(追加失敗ならnull)。
	HoldEquipment* AddEquipment(Item::BaseInfo* info, bool isRandParam);

	// 装備品追加関数
	// 引数：	追加したい装備品のポインタ。
	// 戻り値：	追加成功か。
	inline bool AddEquipment(HoldEquipment* add);

	//指定されたインベントリのリストの先頭を取得。
	 inline const vector<HoldItemBase*>& GetInventoryList( Item::ItemCodeE code) {
		return _InventoryItemList[static_cast<int>(code)];
	}

	 int GetInventoryListSize()
	 {
		 return _InventoryItemList.size();
	 }

	//アイテムコードとIDを元に配列から検索。
	HoldItemBase* FindItem( Item::ItemCodeE code, const unsigned int& id);

	//アイテムを使う。
	void UseItem();

	//所持数を減らす。
	bool SubHoldNum(HoldItemBase* item,int num);

	//インベントリ内を整理(途中にnullを挟まない用に中身を詰めるだけ)。
	void ArrangementInventory();

	//インベントリを整列(ID順になる)。
	void SortID();

	// 指定したアイテムの合計所持数を返却。
	inline int GetHoldNum(Item::ItemCodeE code,int id) {
		return _HoldNumList[static_cast<int>(code)][id];
	}

	//リストの中身を全て削除。
	void deleteList();

	void SaveArmor()
	{
		_OutData(Item::ItemCodeE::Armor);
	}

	void SaveWeapon()
	{
		_OutData(Item::ItemCodeE::Weapon);
	}

private:

	//リストから指定されたアイテムを削除。
	void _DeleteFromList(HoldItemBase* item);

	// 所持品のデータを読み込み。
	void _LoadData();

	//所持品のデータを書き出し。
	// 引数：	アイテムコード。
	void _OutData(Item::ItemCodeE code);

	void SaveMoney()
	{
		picojson::object money;
		money["Money"] = (picojson::value)(double)_PlayerMoney;
		JsonData MoneyData;
		MoneyData.SetDataObject("Money", money);
		MoneyData.Save("Money");
	}

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

	bool _IsAddErrorFlag = false;		//アイテムを追加出来なかった(インベントリが一杯で追加しようとするとtrueになる)。
};
