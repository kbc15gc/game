#pragma once

namespace PlayerInventory
{
	struct BaseInfo :public Noncopyable
	{

	};
	//インベントリ内にあるアイテム。
	//アイテムの情報をまとめた構造体。
	struct ItemInfo :BaseInfo
	{
		int TypeID;				//種類。
		int ID;					//アイテムID。
		char Name[256];			//アイテム名。
		char Description[256];	//アイテムの説。
		int Value;				//値段。
		int Recovery;			//薬草を使った時の回復量。
		int AtkBuff;			//薬草を使った時の攻撃力の上昇量。
		int DefBuff;			//薬草を使った時の防御力の上昇量。
		int SpeedBuff;			//薬草を使った時の移動速度の上昇量。
		int HoldNum;			//所持数。
	};

	//インベントリ内にある防具。
	//防具の情報をまとめた構造体。
	struct ArmorInfo :BaseInfo
	{
		int TypeID;				//種類。
		int ID;					//防具ID。
		char Name[256];			//防具名。
		char Description[256];	//防具の説。
		int Value;				//値段。
		int ATK;				//防具を装備した時に上がる攻撃力。
		int DEF;				//防具を装備した時に上がる防御力。
		int HoldNum;			//所持数。
	};

	//インベントリ内にある武器。。
	//プレイヤーが持っている武器の情報をまとめた構造体。
	struct WeaponInfo :BaseInfo
	{
		int TypeID;				//種類。
		int ID;					//武器ID。
		char Name[256];			//武器名。
		char Description[256];	//武器の説。
		int Value;				//値段。
		int ATK;				//武器を装備した時に上がる攻撃力。
		int HoldNum;			//所持数。
	};
}
//インベントリの枠数。
const int INVENTORYLISTNUM = 5;

#include"GameObject\Village\ItemManager.h"

//インベントリクラス。
class Inventory
{
private:
	Inventory();

	//プレイヤーのアイテムのインベントリを初期化。
	void _PlayerItemListInitialize(int i);

	//プレイヤーの防具のインベントリを初期化。
	void _PlayerArmorListInitialize(int i);

	//プレイヤーの武器のインベントリを初期化。
	void _PlayerWeaponListInitialize(int i);

	//アイテムをインベントリに追加。
	void _AddItem(Item::ItemInfo *item = nullptr);

	//防具をインベントリに追加。
	void _AddArmor(Item::ArmorInfo *armor = nullptr);

	//武器をインベントリに追加。
	void _AddWeapon(Item::WeaponInfo *weapon = nullptr);

	//追加するアイテムがすでに追加されているかチェックし無ければtrueを返す。
	//第1引数:アイテム、第2引数:防具、第3引数:武器。
	bool _AddCheck(Item::ItemInfo *item = nullptr, Item::ArmorInfo *armor = nullptr, Item::WeaponInfo *weapon = nullptr);
public:

	//インベントリの種類。
	enum class InventoryKodeE
	{
		ItemList = 0,
		ArmorList,
		WeaponList,
	};

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->ListInitalize();
		}
		return _InventoryInstance;
	}

	//各インベントリの初期化。
	void ListInitalize();


	//受け取った情報を元にプレイヤーのアイテムのインベントリに追加。
	void AddPlayerInventoryItem(Item::ItemInfo *item = nullptr);

	//受け取った情報を元にプレイヤーの防具のインベントリに追加。
	void AddPlayerInventoryIArmor(Item::ArmorInfo *armor = nullptr);

	//受け取った情報を元にプレイヤーの武器のインベントリに追加。
	void AddPlayerInventoryWeapon(Item::WeaponInfo *weapon = nullptr);

	//プレイヤーのアイテムのインベントリを取得。
	PlayerInventory::ItemInfo* GetPlayerItemList() {
		return &_PlayerItemList[0];
	}

	//プレイヤーの防具のインベントリを取得。
	PlayerInventory::ArmorInfo* GetPlayerAromorList()
	{
		return &_PlayerArmorList[0];
	}

	//プレイヤーの武器のインベントリを取得。
	PlayerInventory::WeaponInfo* GetPlayerWeaponList()
	{

		return &_PlayerWeapon[0];
	}

	//欲しいインベントリを指定するとそのインベントリを取得。
	PlayerInventory::BaseInfo* GetInventory(InventoryKodeE kode)
	{
		switch (kode)
		{
		case Inventory::InventoryKodeE::ItemList:
			return &_PlayerItemList[0];
			break;
		case Inventory::InventoryKodeE::ArmorList:
			return &_PlayerArmorList[0];
			break;
		case Inventory::InventoryKodeE::WeaponList:
			return &_PlayerWeapon[0];
			break;
		default:
			char error[256];
			sprintf(error, "指定したインベントリコードコードが無効です。");
			MessageBoxA(0, error, "インベントリ取得失敗", MB_ICONWARNING);
			break;
		}
	}

private:
	//プレイヤーのアイテムのインベントリ。
	PlayerInventory::ItemInfo	_PlayerItemList[INVENTORYLISTNUM];

	//プレイヤーの防具のインベントリ。
	PlayerInventory::ArmorInfo	_PlayerArmorList[INVENTORYLISTNUM];

	//プレイヤーの武器のインベントリ。
	PlayerInventory::WeaponInfo	_PlayerWeapon[INVENTORYLISTNUM];

	//アイテム、防具、武器の各インベントリを現在どれだけ使っているかを数える変数。
	int UseItemListCounter, UseArmorListCounter, UseWeaponListCounter = 0;

	static Inventory* _InventoryInstance;
};