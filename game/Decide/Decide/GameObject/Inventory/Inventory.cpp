#include "stdafx.h"
#include "Inventory.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem\ConsumptionItem.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon\HoldWeapon.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

void Inventory::Initialize() {
	AddItem(ItemManager::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(0, ItemManager::ItemCodeE::Item));
	DeleteFromList(ItemManager::ItemCodeE::Item, 0);
}

//アイテムをインベントリに追加。
void Inventory::AddItem(ItemManager::ItemCodeE code, Item::BaseInfo* item) {
	Item::BaseInfo* Item = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];

	//アイテムコードを見て作るアイテムを決める。
	{
		//アイテムマネジャーから追加するアイテムの情報を取得。	
		Item = INSTANCE(ItemManager)->GetItemInfo(item->ID, code);
		switch (code)
		{
			//所持アイテム。
		case ItemManager::ItemCodeE::Item:
			//所持アイテムのインスタンス作成。
			Hold = (ConsumptionItem*)INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("ConsumptionItem", 5);
			break;

			//所持防具。
		case ItemManager::ItemCodeE::Armor:
			//所持防具のインスタンス作成。
			Hold = (HoldArmor*)INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 5);
			break;

			//所持武器。
		case ItemManager::ItemCodeE::Weapon:
			//所持武器のインスタンス作成。
			Hold = (HoldWeapon*)INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 5);
			break;
		default:
			//エラー報告。
			sprintf(error, "無効なアイテムコードが渡されました。");
			MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
			break;
		}
		//所持武器に追加する武器の情報を格納。
		Hold->_Info = Item;

		//追加されたアイテムの情報を格納。
		_InfoList.push_back(Item);
	}

	//追加するアイテムの情報が作られたかチェック。
	if (Hold != nullptr) {

		for (int j = 0; j < INVENTORYLISTNUM; j++)
		{
			//インベントリに何も無いなら追加。
			if (_InventoryItemList[(int)code][j] == NULL) {

				//追加。
				_InventoryItemList[(int)code][j] = Hold;
				_InventoryItemList[(int)code][j]->_Info->HoldNum++;
				return;
			}
			else
			{
				//追加する際に同じアイテムかを見て同じなら所持数増加。
				if (_InventoryItemList[(int)code][j]->_Info->ID == Hold->_Info->ID)
				{
					//所持数更新。
					_InventoryItemList[(int)code][j]->_Info->HoldNum++;
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
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)ItemManager::ItemCodeE::Item][_NowLookItemPos];
}

//アイテムコードとIDを元に配列から検索。
HoldItemBase* Inventory::FindItem(ItemManager::ItemCodeE kode, const unsigned int& id) {

	//配列サイズ分検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//発見。
		if (_InventoryItemList[(int)kode][i]->_Info->ID == id) {
			return _InventoryItemList[(int)kode][i];
		}
	}

	//見つからなかった。
	char error[256];
	sprintf(error, "指定されたアイテムが見つかりませんでした。");
	MessageBoxA(0, error, "アイテムの検索失敗", MB_ICONWARNING);
}

void Inventory::DeleteFromList(ItemManager::ItemCodeE code, const unsigned int& id) {

	//配列サイズ分検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		SAFE_DELETE(_InventoryItemList[(int)code][i]);
	}
}