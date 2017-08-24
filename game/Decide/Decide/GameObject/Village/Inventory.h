#pragma once

//インベントリ内にあるアイテム。
namespace InventoryItem
{
	//アイテムの情報をまとめた構造体。
	struct ItemInfo :Noncopyable
	{
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
}

//インベントリ内にある防具。
namespace InventoryArmor
{
	//防具の情報をまとめた構造体。
	struct ArmorInfo :Noncopyable
	{
		int ID;					//防具ID。
		char Name[256];			//防具名。
		char Description[256];	//防具の説。
		int Value;				//値段。
		int ATK;				//防具を装備した時に上がる攻撃力。
		int DEF;				//防具を装備した時に上がる防御力。
		int HoldNum;			//所持数。
	};
}

//インベントリ内にある武器。。
namespace InventoryWeapon
{
	//プレイヤーが持っている武器の情報をまとめた構造体。
	struct WeaponInfo :Noncopyable
	{
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
public:

	//インベントリの種類。
	enum class InventoryKodeE
	{
		ItemList = 0,
		ArmorList,
		WeaponList,
	};

	////追加するアイテムの種類。
	//enum class AddItemKodeE
	//{
	//	Item = 0,
	//	Armor,
	//	Weapon,
	//};

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->ListInitalize();
		}
		return _InventoryInstance;
	}

	void ListInitalize();

	/*void AddPlayerInventoryList(AddItemKodeE kode,
		Item::ItemInfo *item = nullptr,
		Armor::ArmorInfo *armor = nullptr,
		Weapon::WeaponInfo *weapon = nullptr);*/

	//プレイヤーのアイテムのインベントリに追加。
	void AddPlayerInventoryItem(Item::ItemInfo *item = nullptr);

	//プレイヤーの防具のインベントリに追加。
	void AddPlayerInventoryIArmor(Armor::ArmorInfo *armor = nullptr);

	//プレイヤーの武器のインベントリに追加。
	void AddPlayerInventoryWeapon(Weapon::WeaponInfo *weapon = nullptr);

private:
	//プレイヤーのアイテムのインベントリ。
	InventoryItem::ItemInfo	_PlayerItemList[INVENTORYLISTNUM];

	//プレイヤーの防具のインベントリ。
	InventoryArmor::ArmorInfo	_PlayerArmorList[INVENTORYLISTNUM];

	//プレイヤーの武器のインベントリ。
	InventoryWeapon::WeaponInfo	_PlayerWeapon[INVENTORYLISTNUM];

	//プレイヤーが現在持っているアイテム、防具、武器のそれぞれの数。
	int ItemCounter, ArmorCounter, WeaponCounter = 0;

	static Inventory* _InventoryInstance;
};