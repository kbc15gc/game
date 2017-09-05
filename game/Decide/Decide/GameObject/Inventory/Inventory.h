#pragma once

namespace PlayerInventory
{
	struct BaseInfo :public Noncopyable
	{
		int TypeID;				//種類。
		int ID;					//アイテムID。
		char Name[256];			//アイテム名。
		char Description[256];	//アイテムの説。
		int Value;				//値段。
		int HoldNum;			//所持数。
	};
	//インベントリ内にあるアイテム。
	//アイテムの情報をまとめた構造体。
	struct ItemInfo :BaseInfo
	{
		
		int Recovery;			//薬草を使った時の回復量。
		int AtkBuff;			//薬草を使った時の攻撃力の上昇量。
		int DefBuff;			//薬草を使った時の防御力の上昇量。
		int SpeedBuff;			//薬草を使った時の移動速度の上昇量。
		
	};

	//インベントリ内にある防具。
	//防具の情報をまとめた構造体。
	struct ArmorInfo :BaseInfo
	{
		//int TypeID;				//種類。
		//int ID;					//防具ID。
		//char Name[256];			//防具名。
		//char Description[256];	//防具の説。
		//int Value;				//値段。
		int ATK;				//防具を装備した時に上がる攻撃力。
		int DEF;				//防具を装備した時に上がる防御力。
		//int HoldNum;			//所持数。
	};

	//インベントリ内にある武器。。
	//プレイヤーが持っている武器の情報をまとめた構造体。
	struct WeaponInfo :BaseInfo
	{
		//int TypeID;				//種類。
		//int ID;					//武器ID。
		//char Name[256];			//武器名。
		//char Description[256];	//武器の説。
		//int Value;				//値段。
		int ATK;				//武器を装備した時に上がる攻撃力。
		//int HoldNum;			//所持数。
	};
}
//インベントリの枠数。
const int INVENTORYLISTNUM = 5;

#include"GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include <tuple>


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

	//アイテムをインベントリの空いている場所に追加。
	void _AddItem(int AddPos, Item::ItemInfo *item = nullptr);

	//防具をインベントリに追加。
	void _AddArmor(int AddPos, Item::ArmorInfo *armor = nullptr);

	//武器をインベントリに追加。
	void _AddWeapon(int AddPos, Item::WeaponInfo *weapon = nullptr);

	//追加するアイテムをアイテムのインベントリから探しあれば所持数だけを増やし、なければ空いている場所に追加し、どちらでもなければエラーを出す。
	void _ItemAddCheckAndPos(Item::ItemInfo *item = nullptr);

	//追加する防具を防具のインベントリから探しあれば所持数だけを増やし、なければ空いている場所に追加し、どちらでもなければエラーを出す。
	void _ArmorAddCheckAndPos(Item::ArmorInfo *item = nullptr);

	//追加する武器を武器のインベントリから探しあれば所持数だけを増やし、なければ空いている場所に追加し、どちらでもなければエラーを出す。
	void _WeaponAddCheckAndPos(Item::WeaponInfo *item = nullptr);

public:

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->Initalize();
		}
		return _InventoryInstance;
	}

	//各インベントリの初期化。
	void Initalize();

	//インベントリにアイテムを追加する。
	void AddInventory(Item::BaseInfo *item = nullptr);

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

		return &_PlayerWeaponList[0];
	}

	//欲しいインベントリを指定するとそのインベントリを取得。
	PlayerInventory::BaseInfo* GetInventory(ItemManager::ItemKodeE kode)
	{
		switch (kode)
		{
			//アイテムのインベントリを取得。
		case ItemManager::ItemKodeE::Item:
			return &_PlayerItemList[0];
			break;

			//防具のインベントリを取得。
		case ItemManager::ItemKodeE::Armor:
			return &_PlayerArmorList[0];
			break;

			//武器のインベントリを取得。
		case ItemManager::ItemKodeE::Weapon:
			return &_PlayerWeaponList[0];
			break;

			//例外処理。
		default:
			char error[256];
			sprintf(error, "指定されたアイテムコードが無効です。");
			MessageBoxA(0, error, "指定されたアイテムのインベントリ取得失敗", MB_ICONWARNING);
			break;
		}
	}

	//指定された場所のアイテムを捨てる。
	void ItemThrowAway(int pos) {
		_PlayerItemListInitialize(pos);

		//音再生。
		_AddOrOutSE->Play(false);
	}

	//指定された場所の防具を捨てる。
	void ArmorThrowAway(int pos) {
		_PlayerArmorListInitialize(pos);

		//音再生。
		_AddOrOutSE->Play(false);
	}

	//指定された場所の武器を捨てる。
	void WeaponThrowAway(int pos) {
		_PlayerWeaponListInitialize(pos);

		//音再生。
		_AddOrOutSE->Play(false);
	}

	//アイテムを使う。
	//戻り値は左から回復量、攻撃力バフ、防御力バフ、移動速度バフ。
	//戻り値の受け取り方:int recover,int atk,int def,int speed;tie(recover,atk,def,speed)=UseItem();
	tuple<int, int, int, int> UseItem(int pos);

	//選択されたアイテムのインベントリをずらす。
	void ItemListSelectPosUp() {
		_ItemListSelectPos++;
	}

	//選択されたアイテムのインベントリをずらす。
	void ItemListSelectPosDown() {
		_ItemListSelectPos--;
	}

	//選択された防具のインベントリをずらす。
	void ArmorListSelectPosUp() {
		_ArmorListSelectPos++;
	}

	//選択された防具のインベントリをずらす。
	void ArmorListSelectPosDown() {
		_ArmorListSelectPos--;
	}

	//選択された武器のインベントリをずらす。
	void WeaponListSelectPosUp() {
		_WeaponListSelectPos++;
	}

	//選択された武器のインベントリをずらす。
	void WeaponListSelectPosDown() {
		_WeaponListSelectPos--;
	}

	//プレイヤーの所持金を取得。
	int GetPlayerMoney() {
		return _PlayerMoney;
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

private:
	//プレイヤーのアイテムのインベントリ。
	PlayerInventory::ItemInfo	_PlayerItemList[INVENTORYLISTNUM];

	//プレイヤーの防具のインベントリ。
	PlayerInventory::ArmorInfo	_PlayerArmorList[INVENTORYLISTNUM];

	//プレイヤーの武器のインベントリ。
	PlayerInventory::WeaponInfo	_PlayerWeaponList[INVENTORYLISTNUM];

	//今インベントリのどこを選択しているか。
	int _ItemListSelectPos, _ArmorListSelectPos, _WeaponListSelectPos = 0;

	//所持金。
	int _PlayerMoney = 100;

	//インベントリに追加or取り出す時の音。
	SoundSource* _AddOrOutSE = nullptr;

	static Inventory* _InventoryInstance;
};