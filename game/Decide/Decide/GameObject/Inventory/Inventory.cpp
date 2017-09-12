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
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Item));
	SubHoldNum(FindItem(Item::ItemCodeE::Item,0)->GetInfo(), -1);
}

//アイテムをインベントリに追加。
void Inventory::AddItem(Item::ItemCodeE code, Item::BaseInfo* item) {
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
		case Item::ItemCodeE::Item:
			//所持アイテムのインスタンス作成。
			Hold = (ConsumptionItem*)INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("ConsumptionItem", 5);
			break;

			//所持防具。
		case Item::ItemCodeE::Armor:
			//所持防具のインスタンス作成。
			Hold = (HoldArmor*)INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 5);
			break;

			//所持武器。
		case Item::ItemCodeE::Weapon:
			//所持武器のインスタンス作成。
			Hold = (HoldWeapon*)INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 5);
			break;
		default:
			//エラー報告。
			sprintf(error, "無効なアイテムコードが渡されました。");
			MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
			break;
		}
		//所持品に追加するアイテムの情報を格納。
		Hold->SetInfo(Item);
	}

	//追加するアイテムの情報が作られたかチェック。
	if (Hold != nullptr) {

		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			//インベントリに何も無いなら追加。
			if (_InventoryItemList[(int)code][i] == NULL) {

				//追加。
				_InventoryItemList[(int)code][i] = Hold;

				//所持数更新。
				_InventoryItemList[(int)code][i]->AddHoldNum();

				//追加されたアイテムの情報を格納。
				_InfoList.push_back(Item);
				return;
			}
			else
			{
				//追加する際に同じアイテムかを見て同じなら所持数増加。
				if (_InventoryItemList[(int)code][i]->GetInfo()->ID == Hold->GetInfo()->ID)
				{
					//所持数更新。
					_InventoryItemList[(int)code][i]->AddHoldNum();
					return;
				}
			}

			if (i == INVENTORYLISTNUM)
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
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)Item::ItemCodeE::Item][_NowLookItemPos];
}

//アイテムコードとIDを元に配列から検索。
HoldItemBase* Inventory::FindItem(Item::ItemCodeE kode, const unsigned int& id) {

	//配列サイズ分検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//発見。
		if (_InventoryItemList[(int)kode][i]->GetInfo()->ID == id) {
			return _InventoryItemList[(int)kode][i];
		}
	}

	//見つからなかった。
	char error[256];
	sprintf(error, "指定されたアイテムが見つかりませんでした。");
	MessageBoxA(0, error, "アイテムの検索失敗", MB_ICONWARNING);
}

//リストから指定されたアイテムを削除。
void Inventory::DeleteFromList(HoldItemBase* item) {

	//配列サイズ分検索。
	for (auto itr = _InventoryItemList[(int)item->GetInfo()->TypeID].begin() ; itr != _InventoryItemList[(int)item->GetInfo()->TypeID].end();)
	{
		if (item != *itr) {
			itr++;
		}
		else
		{
			INSTANCE(GameObjectManager)->AddRemoveList(FindItem(item->GetInfo()->TypeID, item->GetInfo()->ID));
			*itr = nullptr;
			//itr = _InventoryItemList[(int)item->GetInfo()->TypeID].erase(itr);
		}
	}

	//情報だけを格納したリストのアイテムを削除。
	for (auto itr = _InfoList.begin(); itr != _InfoList.end();)
	{
		//アイテムコードとIDが一致。
		if (item->GetInfo()->TypeID == (*itr)->TypeID&&
			item->GetInfo()->ID == (*itr)->ID) {

			//削除。
			itr = _InfoList.erase(itr);
			return;
		}
		//不一致。
		else
		{
			itr++;
		}
	}
}

//所持数を減らす。
void Inventory::SubHoldNum(Item::BaseInfo* item, int num) {
	//配列サイズ分検索。
	for (auto itr = _InventoryItemList[(int)item->TypeID].begin(); itr != _InventoryItemList[(int)item->TypeID].end();)
	{
		//IDの一致。
		if (item->ID == (*itr)->GetInfo()->ID) 
		{
			//引数分所持品の数を更新。
			(*itr)->AddHoldNum(num);

			//更新した結果所持数が0になれば破棄。
			if ((*itr)->GetHoldNum() <= 0);
			DeleteFromList(*itr);
			return;
		}
		//不一致。
		else
		{
			itr++;
		}
	}
}