#pragma once
#include "GameObject\ItemManager\ItemManager.h"
class SoundSource;
class HoldItemBase;
class HoldEquipment;

const int INVENTORYLISTNUM = 20;

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
	 inline const vector<unique_ptr<HoldItemBase>>& GetInventoryList( Item::ItemCodeE code) {
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

	//インベントリを整列(リストの途中に空きが無いように中身を詰めるだけ)。
	void AlignmentInventoryList();

private:
	//リストから指定されたアイテムを削除。
	void _DeleteFromList(HoldItemBase* item);

	//アイテムリストのデータを保存。
	void _ItemListOutData();

private:
	
	//インベントリ。
	vector<vector<unique_ptr<HoldItemBase>>> _InventoryItemList;

	//今見ているアイテム。
	int _NowLookItemPos = -1;

	//所持金。
	int _PlayerMoney = 100;

	GameObject* _Player;

	static Inventory* _InventoryInstance;
};
