#pragma once

class SoundSource;
class ItemManager;
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
	void AddItem(ItemManager::ItemKodeE kode, Item::BaseInfo* item);

	//指定されたインベントリのリストの先頭を取得。
	 inline const vector<HoldItemBase*>& GetInventoryList(ItemManager::ItemKodeE kode) {
		return _InventoryItemList[(int)kode];
	}

	 //アイテムコードとIDを元に配列から検索。
	 HoldItemBase* FindItem(ItemManager::ItemKodeE kode, const unsigned int& id);

	void UseItem();

private:
	
	//インベントリ。
	vector<vector<HoldItemBase*>> _InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(ItemManager::ItemKodeE::Max), vector<HoldItemBase*>(5, nullptr));

	//今見ているアイテム。
	int _NowLookItemPos = -1;

	//所持金。
	int _PlayerMoney = 100;

	static Inventory* _InventoryInstance;
};