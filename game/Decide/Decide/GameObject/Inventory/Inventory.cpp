#include "stdafx.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "Inventory.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "ConsumptionItem.h"
#include "HoldArmor.h"
#include "HoldWeapon.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

void Inventory::Initialize() {
	for (int i = 0; i < 2; i++)
	{
		AddItem(ItemManager::ItemKodeE::Item, INSTANCE(ItemManager)->GetItemInfo(i, ItemManager::ItemKodeE::Item));
	}
	//_NowLookItemPos = 0;
	//UseItem();
}

//アイテムをインベントリに追加。
void Inventory::AddItem(ItemManager::ItemKodeE kode, Item::BaseInfo* item) {
	Item::BaseInfo* Item = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];

	//アイテムコードを見て作るアイテムを決める。
	switch (kode)
	{
		//所持アイテム。
	case ItemManager::ItemKodeE::Item:

		//アイテムマネジャーから追加するアイテムの情報を取得。	
		Item = (Item::ItemInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, kode);
		//所持アイテムのインスタンス作成。
		Hold = (ConsumptionItem*)INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("ConsumptionItem", 5);
		//所持アイテムに追加するアイテムの情報を格納。
		Hold->_Info = Item;
		break;

		//所持防具。
	case ItemManager::ItemKodeE::Armor:

		//アイテムマネジャーから追加する防具の情報を取得。	
		Item = (Item::ArmorInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, kode);
		//所持防具のインスタンス作成。
		Hold = (HoldArmor*)INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 5);
		//所持防具に追加する防具の情報を格納。
		Hold->_Info = Item;
		break;

		//所持武器。
	case ItemManager::ItemKodeE::Weapon:

		//アイテムマネジャーから追加する武器の情報を取得。	
		Item = (Item::WeaponInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, kode);;
		//所持武器のインスタンス作成。
		Hold = (HoldWeapon*)INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 5);
		//所持武器に追加する武器の情報を格納。
		Hold->_Info = Item;
		break;

	case ItemManager::ItemKodeE::Max:

		//エラー報告。
		sprintf(error, "無効なアイテムコードが渡されました。");
		MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
		break;
	default:
		//エラー報告。
		sprintf(error, "無効なアイテムコードが渡されました。");
		MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
		break;
	}

	//追加するアイテムの情報が作られたかチェック。
	if (Hold != nullptr) {

		for (int j = 0; j < INVENTORYLISTNUM; j++)
		{
			//インベントリに何も無いなら追加。
			if (_InventoryItemList[(int)kode][j] == NULL) {

				//追加。
				_InventoryItemList[(int)kode][j] = Hold;
				_InventoryItemList[(int)kode][j]->_Info->HoldNum++;
				return;
			}
			else
			{
				//追加する際に同じアイテムかを見て同じなら所持数増加。
				if (_InventoryItemList[(int)kode][j]->_Info->ID == Hold->_Info->ID)
				{
					//所持数更新。
					_InventoryItemList[(int)kode][j]->_Info->HoldNum++;
					return;
				}
			}

			if (j == INVENTORYLISTNUM)
			{
				//エラー報告。
				char error[256];
				sprintf(error, "インベントリが一杯で追加されませんでした。");
				MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
			}
		}
	}
}

void Inventory::UseItem() {
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)ItemManager::ItemKodeE::Item][_NowLookItemPos];
}

HoldItemBase* Inventory::FindItem(ItemManager::ItemKodeE kode, const unsigned int& id) {
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[(int)kode][i]->_Info->ID == id) {
			return _InventoryItemList[(int)kode][i];
		}
	}
	return nullptr;
}