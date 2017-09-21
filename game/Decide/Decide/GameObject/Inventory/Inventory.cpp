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
	for (int type = 0; type < static_cast<int>(Item::ItemCodeE::Max); type++) {
		_InventoryItemList.push_back(vector<unique_ptr<HoldItemBase>>());
		for (int idx = 0; idx < INVENTORYLISTNUM; idx++) {
			_InventoryItemList[type].push_back(unique_ptr<HoldItemBase>());
		}
	}

}

void Inventory::Initialize() {

	AddItem(INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Item));
	AddItem(INSTANCE(ItemManager)->GetItemInfo(1, Item::ItemCodeE::Item));

	HoldArmor* armor = new HoldArmor(INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Armor));
	armor->CreateRandParam();

	AddEquipment(armor, Item::ItemCodeE::Armor);

	AddEquipment(new HoldArmor(INSTANCE(ItemManager)->GetItemInfo(1, Item::ItemCodeE::Armor)), Item::ItemCodeE::Armor);

	HoldWeapon* weapon = new HoldWeapon(INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Weapon));
	weapon->CreateRandParam();
	AddEquipment(weapon, Item::ItemCodeE::Weapon);

	//ファイルネーム
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	vector<vector<unique_ptr<HoldInfo>>> work= vector<vector<unique_ptr<HoldInfo>>>(static_cast<int>(Item::ItemCodeE::Max));
	FOR(i, ARRAY_SIZE(filename)) {
		//ファイルパス
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);
		switch (i)
		{
		case static_cast<int>((Item::ItemCodeE::Item)):
			//ファイルからアイテム情報読み込み。
			//Support::LoadCSVData<HoldInfo>(filepath, HoldItemData, ARRAY_SIZE(HoldItemData), work[static_cast<int>((Item::ItemCodeE::Item))]);
			/*for (int idx = 0; idx < work[static_cast<int>((Item::ItemCodeE::Item))].size(); idx++)
			{
			}*/
			break;
		case static_cast<int>((Item::ItemCodeE::Armor)):
			//ファイルから防具情報読み込み。
			//Support::LoadCSVData<HoldInfo>(filepath, HoldArmorData, ARRAY_SIZE(HoldArmorData), work[static_cast<int>((Item::ItemCodeE::Armor))]);
			/*for (int idx = 0; idx < work[static_cast<int>((Item::ItemCodeE::Armor))].size(); idx++)
			{
			}*/
			break;
		case static_cast<int>((Item::ItemCodeE::Weapon)):
			//ファイルから武器情報読み込み。
			//Support::LoadCSVData<HoldInfo>(filepath, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), work[static_cast<int>((Item::ItemCodeE::Weapon))]);
			/*for (int idx = 0; idx < work[static_cast<int>((Item::ItemCodeE::Weapon))].size(); idx++)
			{
			}*/
			break;
		default:
			break;
		}
	}
}

//アイテムをインベントリに追加。
void Inventory::AddItem(Item::BaseInfo* item, int num) {
	Item::BaseInfo* Info = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];


	//アイテムマネジャーから追加するアイテムの情報を取得。	
	Info = INSTANCE(ItemManager)->GetItemInfo(item->ID, Item::ItemCodeE::Item);

	//所持アイテムのインスタンス作成。
	Hold = new ConsumptionItem(Info);

	if (Hold) {
		//所持数更新。
		static_cast<ConsumptionItem*>(Hold)->UpdateHoldNum(num);
	}


	//追加するアイテムの情報が作られたかチェック。
	if (Hold != nullptr) {

		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] && _InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]->GetInfo()->ID == Hold->GetInfo()->ID)
			{
				//同じアイテムが配列にある。
				static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i].get())->UpdateHoldNum(num);

				//所持品の所持数書き出し。
				_ItemListOutData();
				return;
			}
		}

		// 追加するアイテムが配列になかった。
		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i].get() == nullptr) {
				//インベントリに空きがある。

				// 新しく作成したアイテムを配列に追加。
				_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i].reset(Hold);

				//所持品配列の所持数をCSVに書き出し。
				_ItemListOutData();

				return;
			}
		}


		{
			//エラー報告。
			char error[256];
			sprintf(error, "インベントリが一杯で追加されませんでした。");
			MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
		}
	}
}

void Inventory::UseItem() {
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][_NowLookItemPos];
}

//アイテムコードとIDを元に配列から検索。
HoldItemBase* Inventory::FindItem(Item::ItemCodeE code, const unsigned int& id) {

	//配列サイズ分検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//発見。
		if (_InventoryItemList[static_cast<int>(code)][i] && _InventoryItemList[static_cast<int>(code)][i]->GetInfo()->ID == id) {
			return _InventoryItemList[static_cast<int>(code)][i].get();
		}
	}

	//見つからなかった。
	char error[256];
	sprintf(error, "指定されたアイテムが見つかりませんでした。");
	MessageBoxA(0, error, "アイテムの検索失敗", MB_ICONWARNING);
}

//リストから指定されたアイテムを削除。
void Inventory::_DeleteFromList(HoldItemBase* item) {

	//配列サイズ分検索。
	for (auto itr = _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)].begin() ; itr != _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)].end();)
	{
		//中身が無いまたは不一致。
		if (item != (*itr).get()) {
			itr++;
		}
		else
		{
			//一致したので中身を削除。
			(*itr).reset(nullptr);
			return;
		}
	}
}

//所持数を減らす。
bool Inventory::SubHoldNum(Item::BaseInfo* item, int num) {
	//配列サイズ分検索。
	for (auto itr = _InventoryItemList[static_cast<int>(item->TypeID)].begin(); itr != _InventoryItemList[static_cast<int>(item->TypeID)].end();)
	{
		//IDの一致。
		if ((*itr) != nullptr&&item->ID == (*itr)->GetInfo()->ID)
		{
			//引数分所持品の数を更新。
			static_cast<ConsumptionItem*>((*itr).get())->UpdateHoldNum(-num);

			//更新した結果所持数が0になれば破棄。
			if (static_cast<ConsumptionItem*>((*itr).get())->GetHoldNum() <= 0) {
				//リストから削除。
				_DeleteFromList((*itr).get());
			}
			//所持品の所持数書き出し。
			_ItemListOutData();
			return true;
		}
		//不一致。
		else
		{
			itr++;
		}
	}
	return false;
}

void Inventory::_ItemListOutData() {

	//ファイルネーム。
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };

	//CSVに書き出すようのリスト。
	vector<vector<unique_ptr<HoldInfo>>> OutPutCSVList = vector<vector<unique_ptr<HoldInfo>>>(static_cast<int>(Item::ItemCodeE::Max));
	FOR(i, ARRAY_SIZE(filename))
	{
		//ファイルパス。
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);
		switch (i)
		{
		//アイテム。
		case static_cast<int>((Item::ItemCodeE::Item)):
			for (int idx = 0; idx < _InventoryItemList[static_cast<int>((Item::ItemCodeE::Item))].size(); idx++) {
				if (_InventoryItemList[static_cast<int>((Item::ItemCodeE::Item))][idx]) {
				
					//持っているアイテムの情報を書き出し用のリストに追加。
					OutPutCSVList[static_cast<int>(Item::ItemCodeE::Item)].push_back(
						unique_ptr<HoldInfo>(
							new HoldInfo(
								static_cast<int>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][idx]->GetInfo()->TypeID),
								_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][idx]->GetInfo()->ID,
								static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][idx].get())->GetHoldNum()
							)
							));
				}
			}
			//ItemList.CSVに書き出し。
			Support::OutputCSV<HoldInfo>(filepath, HoldItemData, ARRAY_SIZE(HoldItemData), OutPutCSVList[static_cast<int>(Item::ItemCodeE::Item)]);
			break;

		//防具。
		case static_cast<int>(Item::ItemCodeE::Armor):
			for (int idx = 0; idx < _InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)].size(); idx++) {
				if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)][idx]) {

					//持っている防具の情報を書き出し用のリストに追加。
					OutPutCSVList[static_cast<int>(Item::ItemCodeE::Armor)].push_back(
						unique_ptr<HoldArmorInfo>(
							new HoldArmorInfo(
								static_cast<int>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)][idx]->GetInfo()->TypeID),
								_InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)][idx]->GetInfo()->ID,
								static_cast<HoldArmor*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)][idx].get())->GetDefRnd(),
								static_cast<HoldArmor*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)][idx].get())->GetMDefRnd()
							)
							));
				}
			}

			//ArmorList.CSVに書き出し。
			Support::OutputCSV<HoldInfo>(filepath, HoldArmorData, ARRAY_SIZE(HoldArmorData), OutPutCSVList[static_cast<int>(Item::ItemCodeE::Armor)]);
			break;

		//武器。
		case static_cast<int>(Item::ItemCodeE::Weapon):

			for (int idx = 0; idx < _InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)].size(); idx++) {
				if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)][idx]) {

					//持っている武器の情報を書き出し用のリストに追加。
					OutPutCSVList[static_cast<int>(Item::ItemCodeE::Weapon)].push_back(
						unique_ptr<HoldWeponInfo>(
							new HoldWeponInfo(
								static_cast<int>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)][idx]->GetInfo()->TypeID),
								_InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)][idx]->GetInfo()->ID,
								static_cast<HoldWeapon*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)][idx].get())->GetAtkRnd(),
								static_cast<HoldWeapon*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)][idx].get())->GetMtkRnd(),
								static_cast<HoldWeapon*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)][idx].get())->GetCrtRnd()
							)
							));
				}
			}
			//WeaponList.CSVに書き出し。
			Support::OutputCSV<HoldInfo>(filepath, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), OutPutCSVList[static_cast<int>(Item::ItemCodeE::Weapon)]);
			break;
		}
	}
}

//装備品の追加。
void Inventory::AddEquipment(HoldEquipment* equi, Item::ItemCodeE code) {
	if (code == Item::ItemCodeE::Item) {
		//アイテムは別の追加関数を使って。
		char error[256];
		sprintf(error, "装備の追加でcode アイテムが指定されました。");
		MessageBoxA(0, error, "装備品追加失敗", MB_ICONWARNING);
		abort();
	}

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((code))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(code)][idx] == nullptr) {

			//装備品を追加。
			_InventoryItemList[static_cast<int>(code)][idx].reset(equi);

			//所持品の情報を書き出し。
			_ItemListOutData();
			break;
		}
	}	
}