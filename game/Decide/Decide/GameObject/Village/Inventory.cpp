#include "stdafx.h"
#include "Inventory.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

void Inventory::ListInitalize()
{
	//ファイルネーム
	const char* filename[] = { "Item","Armor","Weapon", };
	FOR(i, ARRAY_SIZE(filename)) {

		//各リストの初期化。
		{
			switch (i)
			{
			case (int)InventoryKodeE::ItemList:
				//プレイヤーのアイテムのインベントリ。
				for (int j = 0; j < INVENTORYLISTNUM; j++)
				{
					_PlayerItemList[j].ID = -1;
					strcpy(_PlayerItemList[j].Name, "None");
					strcpy(_PlayerItemList[j].Description, "None");
					_PlayerItemList[j].Value = -1;
					_PlayerItemList[j].Recovery = -1;
					_PlayerItemList[j].AtkBuff = -1;
					_PlayerItemList[j].DefBuff = -1;
					_PlayerItemList[j].HoldNum = 0;
					_PlayerItemList[j].SpeedBuff = -1;
				}
				break;
			case (int)InventoryKodeE::ArmorList:
				//プレイヤーの防具のインベントリ。
				for (int j = 0; j < INVENTORYLISTNUM; j++)
				{
					_PlayerArmorList[j].ID = -1;
					strcpy(_PlayerArmorList[j].Name, "None");
					strcpy(_PlayerArmorList[j].Description, "None");
					_PlayerArmorList[j].Value = -1;
					_PlayerArmorList[j].ATK = -1;
					_PlayerArmorList[j].DEF = -1;
					_PlayerArmorList[j].HoldNum = 0;
				}
				break;
			case (int)InventoryKodeE::WeaponList:
				//プレイヤーの武器のインベントリ。
				for (int j = 0; j < INVENTORYLISTNUM; j++)
				{
					_PlayerWeapon[j].ID = -1;
					strcpy(_PlayerWeapon[j].Name, "None");
					strcpy(_PlayerWeapon[j].Description, "None");
					_PlayerWeapon[j].Value = -1;
					_PlayerWeapon[j].ATK = -1;
					_PlayerWeapon[j].HoldNum = 0;
				}
				break;
			}
		}
	}
}

//void Inventory::AddPlayerInventoryList(AddItemKodeE kode,Item::ItemInfo *item, Armor::ArmorInfo *armor, Weapon::WeaponInfo *weapon)
//{
//	switch (kode)
//	{
//		//プレイヤーのアイテムのインベントリに追加。
//	case Inventory::AddItemKodeE::Item:
//
//		//所持しているアイテムの中に追加するアイテムと同じものがないかチェック。
//		for (int i = 0; i < ItemCounter; i++)
//		{
//			//同じアイテムなので所持数の更新。
//			if (_PlayerItemList[i].ID = item->ID)
//			{
//				_PlayerItemList[i].HoldNum++;
//				break;
//			}	
//		}
//
//		//追加するアイテムの情報を追加。
//		{
//			_PlayerItemList[ItemCounter].ID = item->ID;
//			strcpy(_PlayerItemList[ItemCounter].Name, item->Name);
//			strcpy(_PlayerItemList[ItemCounter].Description, item->Description);
//			_PlayerItemList[ItemCounter].Value = item->Value;
//			_PlayerItemList[ItemCounter].Recovery = item->Recovery;
//			_PlayerItemList[ItemCounter].AtkBuff = item->AtkBuff;
//			_PlayerItemList[ItemCounter].DefBuff = item->DefBuff;
//			_PlayerItemList[ItemCounter].SpeedBuff = item->SpeedBuff;
//			_PlayerItemList[ItemCounter].HoldNum++;
//			ItemCounter++;
//		}
//		break;
//
//		//プレイヤーの防具のインベントリに追加。
//	case Inventory::AddItemKodeE::Armor:
//
//		//所持している防具の中に追加する防具と同じものがないかチェック。
//		for (int i = 0; i < ArmorCounter; i++)
//		{
//			//同じ防具なので所持数の更新。
//			if (_PlayerArmorList[i].ID = item->ID)
//			{
//				_PlayerArmorList[i].HoldNum++;
//				break;
//			}
//		}
//
//		//追加する防具の情報を追加。
//		{
//			_PlayerArmorList[ArmorCounter].ID = armor->ID;
//			strcpy(_PlayerArmorList[ArmorCounter].Name, armor->Name);
//			strcpy(_PlayerArmorList[ArmorCounter].Description, armor->Description);
//			_PlayerArmorList[ArmorCounter].Value = armor->Value;
//			_PlayerArmorList[ArmorCounter].ATK = armor->ATK;
//			_PlayerArmorList[ArmorCounter].DEF = armor->DEF;
//		}
//		break;
//
//		//プレイヤーの武器のインベントリに追加。
//	case Inventory::AddItemKodeE::Weapon:
//
//		//所持している武器の中に追加する武器と同じものがないかチェック。
//		for (int i = 0; i < ArmorCounter; i++)
//		{
//			//同じ武器なので所持数の更新。
//			if (_PlayerWeapon[i].ID = item->ID)
//			{
//				_PlayerWeapon[i].HoldNum++;
//				break;
//			}
//		}
//
//		//追加する武器の情報を追加。
//		{
//			_PlayerWeapon[WeaponCounter].ID = armor->ID;
//			strcpy(_PlayerWeapon[WeaponCounter].Name, armor->Name);
//			strcpy(_PlayerArmorList[WeaponCounter].Description, armor->Description);
//			_PlayerWeapon[WeaponCounter].Value = armor->Value;
//			_PlayerWeapon[WeaponCounter].ATK = armor->ATK;
//		}
//		break;
//	default:
//		char error[256];
//		sprintf(error, "指定したアイテムコードが無効です。");
//		MessageBoxA(0, error, "アイテムコードの指定を失敗", MB_ICONWARNING);
//		break;
//	}
//}

void Inventory::AddPlayerInventoryItem(Item::ItemInfo *item)
{
	//所持しているアイテムの中に追加するアイテムと同じものがないかチェック。
	for (int i = 0; i < ItemCounter; i++)
	{
		//同じアイテムなので所持数の更新。
		if (_PlayerItemList[i].ID = item->ID)
		{
			_PlayerItemList[i].HoldNum++;
			break;
		}
	}

	//追加するアイテムの情報を追加。
	{
		_PlayerItemList[ItemCounter].ID = item->ID;
		strcpy(_PlayerItemList[ItemCounter].Name, item->Name);
		strcpy(_PlayerItemList[ItemCounter].Description, item->Description);
		_PlayerItemList[ItemCounter].Value = item->Value;
		_PlayerItemList[ItemCounter].Recovery = item->Recovery;
		_PlayerItemList[ItemCounter].AtkBuff = item->AtkBuff;
		_PlayerItemList[ItemCounter].DefBuff = item->DefBuff;
		_PlayerItemList[ItemCounter].SpeedBuff = item->SpeedBuff;
		_PlayerItemList[ItemCounter].HoldNum++;
		ItemCounter++;
	}
}

void Inventory::AddPlayerInventoryIArmor(Armor::ArmorInfo *armor)
{
	//所持している防具の中に追加する防具と同じものがないかチェック。
	for (int i = 0; i < ArmorCounter; i++)
	{
		//同じ防具なので所持数の更新。
		if (_PlayerArmorList[i].ID = armor->ID)
		{
			_PlayerArmorList[i].HoldNum++;
			break;
		}
	}

	//追加する防具の情報を追加。
	{
		_PlayerArmorList[ArmorCounter].ID = armor->ID;
		strcpy(_PlayerArmorList[ArmorCounter].Name, armor->Name);
		strcpy(_PlayerArmorList[ArmorCounter].Description, armor->Description);
		_PlayerArmorList[ArmorCounter].Value = armor->Value;
		_PlayerArmorList[ArmorCounter].ATK = armor->ATK;
		_PlayerArmorList[ArmorCounter].DEF = armor->DEF;
	}
}

void Inventory::AddPlayerInventoryWeapon(Weapon::WeaponInfo *weapon)
{
	//所持している武器の中に追加する武器と同じものがないかチェック。
	for (int i = 0; i < ArmorCounter; i++)
	{
		//同じ武器なので所持数の更新。
		if (_PlayerWeapon[i].ID = weapon->ID)
		{
			_PlayerWeapon[i].HoldNum++;
			break;
		}
	}

	//追加する武器の情報を追加。
	{
		_PlayerWeapon[WeaponCounter].ID = weapon->ID;
		strcpy(_PlayerWeapon[WeaponCounter].Name, weapon->Name);
		strcpy(_PlayerArmorList[WeaponCounter].Description, weapon->Description);
		_PlayerWeapon[WeaponCounter].Value = weapon->Value;
		_PlayerWeapon[WeaponCounter].ATK = weapon->ATK;
	}
}