#pragma once
#include "GameObject\ItemManager\ItemManager.h"
class SoundSource;
class HoldItemBase;

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
	void AddItem(Item::ItemCodeE code, Item::BaseInfo* item, int num = 1);

	//指定されたインベントリのリストの先頭を取得。
	 inline const vector<unique_ptr<HoldItemBase>>& GetInventoryList( Item::ItemCodeE code) {
		return _InventoryItemList[(int)code];
	}


	 //アイテムコードとIDを元に配列から検索。
	 HoldItemBase* FindItem( Item::ItemCodeE code, const unsigned int& id);


	 // 西にプレゼント　　by のぼりおはぁと。

	// //追加されたアイテムの情報だけを取得。
	//inline unique_ptr<vector<HoldItemBase*>> GetInfoList() {

	//	unique_ptr<vector<HoldItemBase*>> info(new vector<HoldItemBase*>());
	//	for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++) {
	//		for (int idx2 = 0; idx2 < _InventoryItemList[idx].size();idx2++) {
	//			if (_InventoryItemList[idx][idx2]) {
	//				info->push_back(_InventoryItemList[idx][idx2].get());
	//			}
	//		}
	//	}
	//	
	//	return move(info);
	//}
	
	//アイテムを使う。
	void UseItem();

	//所持数を減らす。
	bool SubHoldNum(Item::BaseInfo* item,int num);

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

	static Inventory* _InventoryInstance;
};
