#pragma once
#include "GameObject\ItemManager\ItemManager.h"
class SoundSource;
class HoldItemBase;

const int INVENTORYLISTNUM = 5;

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

	//アイテムをインベントリに追加。
	void AddItem(ItemManager::ItemCodeE code, Item::BaseInfo* item);

	//指定されたインベントリのリストの先頭を取得。
	inline const vector<HoldItemBase*>& GetInventoryList(ItemManager::ItemCodeE code) {
		return _InventoryItemList[(int)code];
	}

	 //アイテムコードとIDを元に配列から検索。
	 HoldItemBase* FindItem(ItemManager::ItemCodeE code, const unsigned int& id);

	
	//リストから指定されたアイテムを削除。
	void DeleteFromList(ItemManager::ItemCodeE code, const unsigned int& id);

	inline vector<Item::BaseInfo*> GetInfoList() {
		return _InfoList;
	}
	
	void UseItem();
private:
	
	//インベントリ。
	vector<vector<HoldItemBase*>> _InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(ItemManager::ItemCodeE::Max), vector<HoldItemBase*>(INVENTORYLISTNUM, nullptr));

	//インベントリのアイテムの情報。
	vector<Item::BaseInfo*> _InfoList;

	//今見ているアイテム。
	int _NowLookItemPos = -1;

	//所持金。
	int _PlayerMoney = 100;

	static Inventory* _InventoryInstance;
};