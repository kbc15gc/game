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
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(1, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(2, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(3, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(4, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(5, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(6, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(7, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(8, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(9, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(10, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(10, Item::ItemCodeE::Item));
	AddItem(Item::ItemCodeE::Armor, INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Armor));
	AddItem(Item::ItemCodeE::Armor, INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Armor));
	AddItem(Item::ItemCodeE::Armor, INSTANCE(ItemManager)->GetItemInfo(1, Item::ItemCodeE::Armor));
	AddItem(Item::ItemCodeE::Weapon, INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Weapon));
	AddItem(Item::ItemCodeE::Weapon, INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Weapon));
	AddItem(Item::ItemCodeE::Weapon, INSTANCE(ItemManager)->GetItemInfo(1, Item::ItemCodeE::Weapon));


	////ファイルパス
	//char filepath[256] = "";
	//strcpy(filepath, "Asset/Data/InventoryData/ItemList.csv");
	////ファイルからアイテム情報読み込み。
	//vector<unique_ptr<HoldInfo>> work;
	//Support::LoadCSVData<HoldInfo>(filepath, HoldItemData, ARRAY_SIZE(HoldItemData), work);
	//for (auto itr = work.begin();itr!=work.end();)
	//{
	//	if ((*itr) != nullptr) {
	//		AddItem(static_cast<Item::ItemCodeE>((*itr)->_TypeID),INSTANCE(ItemManager)->GetItemInfo((*itr)->_ID, static_cast<Item::ItemCodeE>((*itr)->_TypeID)), (*itr)->_HoldNum);
	//	}
	//	else
	//	{
	//		itr++;
	//	}
	//}

	////ファイルパス
	//char filepath2[256] = "";
	//strcpy(filepath2, "Asset/Data/InventoryData/ArmorList.csv");
	////ファイルからアイテム情報読み込み。
	//vector<unique_ptr<HoldArmorInfo>> work2;
	//Support::LoadCSVData<HoldArmorInfo>(filepath2, HoldArmorData, ARRAY_SIZE(HoldArmorData), work2);


	////ファイルパス
	//char filepath3[256] = "";
	//strcpy(filepath3, "Asset/Data/InventoryData/WeaponList.csv");
	////ファイルからアイテム情報読み込み。
	//vector<unique_ptr<HoldWeponInfo>> work3;
	//Support::LoadCSVData<HoldWeponInfo>(filepath3, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), work3);


}

//アイテムをインベントリに追加。
void Inventory::AddItem(Item::ItemCodeE code, Item::BaseInfo* item,int num) {
	Item::BaseInfo* Info = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];

	//アイテムコードを見て作るアイテムを決める。
	{
		//アイテムマネジャーから追加するアイテムの情報を取得。	
		Info = INSTANCE(ItemManager)->GetItemInfo(item->ID, code);
		switch (code)
		{
		case Item::ItemCodeE::Item:
			//所持アイテム。

			//所持アイテムのインスタンス作成。
			Hold = new ConsumptionItem(Info);
			break;

		case Item::ItemCodeE::Armor:
			//所持防具。

			//所持防具のインスタンス作成。
			Hold = new HoldArmor(Info);
			break;

		case Item::ItemCodeE::Weapon:
			//所持武器。

			//所持武器のインスタンス作成。
			Hold = new HoldWeapon(Info);
			break;
		default:
			//エラー報告。
			sprintf(error, "無効なアイテムコードが渡されました。");
			MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
			break;
		}

		if (Hold) {
			//所持数更新。
			Hold->UpdateHoldNum(num);
		}
	}

	//追加するアイテムの情報が作られたかチェック。
	if (Hold != nullptr) {

		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[(int)code][i] && _InventoryItemList[(int)code][i]->GetInfo()->ID == Hold->GetInfo()->ID)
			{
				//同じアイテムが配列にある。
				_InventoryItemList[(int)code][i]->UpdateHoldNum(num);

				//所持品の所持数書き出し。
				_ItemListOutData();
				return;
			}
		}

		// 追加するアイテムが配列になかった。
		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[(int)code][i].get() == nullptr) {
				//インベントリに空きがある。

				// 新しく作成したアイテムを配列に追加。
				_InventoryItemList[(int)code][i].reset(Hold);

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
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)Item::ItemCodeE::Item][_NowLookItemPos];
}

//アイテムコードとIDを元に配列から検索。
HoldItemBase* Inventory::FindItem(Item::ItemCodeE kode, const unsigned int& id) {

	//配列サイズ分検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//発見。
		if (_InventoryItemList[(int)kode][i] && _InventoryItemList[(int)kode][i]->GetInfo()->ID == id) {
			return _InventoryItemList[(int)kode][i].get();
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
	for (auto itr = _InventoryItemList[(int)item->GetInfo()->TypeID].begin() ; itr != _InventoryItemList[(int)item->GetInfo()->TypeID].end();)
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
	for (auto itr = _InventoryItemList[(int)item->TypeID].begin(); itr != _InventoryItemList[(int)item->TypeID].end();)
	{
		//IDの一致。
		if ((*itr) != nullptr&&item->ID == (*itr)->GetInfo()->ID)
		{
			//引数分所持品の数を更新。
			(*itr)->UpdateHoldNum(-num);

			//更新した結果所持数が0になれば破棄。
			if ((*itr)->GetHoldNum() <= 0) {
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

	////ファイルネーム
	//const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	//vector<unique_ptr<HoldInfo>> list;
	//FOR(i, ARRAY_SIZE(filename))
	//{
	//	//ファイルパス
	//	char filepath[256] = "";
	//	sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);
	//	switch (i)
	//	{
	//	case (int)Item::ItemCodeE::Item:
	//		for (int idx = 0; idx < _InventoryItemList[(int)Item::ItemCodeE::Item].size(); idx++) {
	//			if (_InventoryItemList[(int)Item::ItemCodeE::Item][idx]) {
	//				list.push_back(
	//					unique_ptr<HoldInfo>(
	//						new HoldInfo(
	//							static_cast<int>(_InventoryItemList[(int)Item::ItemCodeE::Item][idx]->GetInfo()->TypeID),
	//							_InventoryItemList[(int)Item::ItemCodeE::Item][idx]->GetInfo()->ID,
	//							_InventoryItemList[(int)Item::ItemCodeE::Item][idx]->GetHoldNum()
	//						)
	//						));
	//			}
	//		}
	//		Support::OutputCSV<HoldInfo>(filepath, HoldItemData, ARRAY_SIZE(HoldItemData), list);
	//		break;

	//	case (int)Item::ItemCodeE::Armor:
	//		for (int idx = 0; idx < _InventoryItemList[(int)Item::ItemCodeE::Item].size(); idx++) {
	//			if (_InventoryItemList[(int)Item::ItemCodeE::Item][idx]) {
	//				list.push_back(
	//					unique_ptr<HoldArmorInfo>(
	//						new HoldArmorInfo(
	//							static_cast<int>(_InventoryItemList[(int)Item::ItemCodeE::Armor][idx]->GetInfo()->TypeID),
	//							_InventoryItemList[(int)Item::ItemCodeE::Armor][idx]->GetInfo()->ID,
	//							_InventoryItemList[(int)Item::ItemCodeE::Armor][idx]->GetHoldNum(),
	//							static_cast<HoldArmor*>(_InventoryItemList[(int)Item::ItemCodeE::Armor][idx].get())->GetDefRnd(),
	//							static_cast<HoldArmor*>(_InventoryItemList[(int)Item::ItemCodeE::Armor][idx].get())->GetMDefRnd()
	//						)
	//						));
	//			}
	//		}
	//		Support::OutputCSV<HoldInfo>(filepath, HoldArmorData, ARRAY_SIZE(HoldArmorData), list);
	//		break;

	//	case (int)Item::ItemCodeE::Weapon:

	//		for (int idx = 0; idx < _InventoryItemList[(int)Item::ItemCodeE::Weapon].size(); idx++) {
	//			if (_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]) {
	//				list.push_back(
	//					unique_ptr<HoldWeponInfo>(
	//						new HoldWeponInfo(
	//							static_cast<int>(_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]->GetInfo()->TypeID),
	//							_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]->GetInfo()->ID,
	//							_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]->GetHoldNum(),
	//							static_cast<HoldWeapon*>(_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx].get())->GetAtkRnd(),
	//							static_cast<HoldWeapon*>(_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx].get())->GetMtkRnd()
	//						)
	//						));
	//			}
	//		}
	//		Support::OutputCSV<HoldInfo>(filepath, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), list);
	//		break;
	//	}
	//}

	//vector<unique_ptr<HoldInfo>> work1;
	//for (int idx = 0; idx < _InventoryItemList[(int)Item::ItemCodeE::Item].size(); idx++) {
	//	if (_InventoryItemList[(int)Item::ItemCodeE::Item][idx]) {
	//		work1.push_back(
	//			unique_ptr<HoldInfo>(
	//				new HoldInfo(
	//					static_cast<int>(_InventoryItemList[(int)Item::ItemCodeE::Item][idx]->GetInfo()->TypeID),
	//					_InventoryItemList[(int)Item::ItemCodeE::Item][idx]->GetInfo()->ID,
	//					_InventoryItemList[(int)Item::ItemCodeE::Item][idx]->GetHoldNum()
	//				)
	//				));
	//		//ファイルパス
	//		char filepath[256] = "";
	//		strcpy(filepath, "Asset/Data/InventoryData/ItemList.csv");
	//		Support::OutputCSV<HoldInfo>(filepath, HoldItemData, ARRAY_SIZE(HoldItemData), work1);
	//	}
	//}
	//
	//vector<unique_ptr<HoldArmorInfo>> work2;
	//for (int idx = 0; idx < _InventoryItemList[(int)Item::ItemCodeE::Armor].size(); idx++) {
	//	if (_InventoryItemList[(int)Item::ItemCodeE::Armor][idx]) {
	//		work2.push_back(
	//			unique_ptr<HoldArmorInfo>(
	//				new HoldArmorInfo(
	//					static_cast<int>(_InventoryItemList[(int)Item::ItemCodeE::Armor][idx]->GetInfo()->TypeID),
	//					_InventoryItemList[(int)Item::ItemCodeE::Armor][idx]->GetInfo()->ID,
	//					_InventoryItemList[(int)Item::ItemCodeE::Armor][idx]->GetHoldNum(),
	//					static_cast<HoldArmor*>(_InventoryItemList[(int)Item::ItemCodeE::Armor][idx].get())->GetDefRnd(),
	//					static_cast<HoldArmor*>(_InventoryItemList[(int)Item::ItemCodeE::Armor][idx].get())->GetMDefRnd()
	//				)));
	//		//ファイルパス
	//		char filepath2[256] = "";
	//		strcpy(filepath2, "Asset/Data/InventoryData/ArmorList.csv");
	//		Support::OutputCSV<HoldArmorInfo>(filepath2, HoldArmorData, ARRAY_SIZE(HoldArmorData), work2);
	//	}
	//}
	//
	//vector<unique_ptr<HoldWeponInfo>> work3;
	//for (int idx = 0; idx < _InventoryItemList[(int)Item::ItemCodeE::Weapon].size(); idx++) {
	//	if (_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]) {
	//		work3.push_back(
	//			unique_ptr<HoldWeponInfo>(
	//				new HoldWeponInfo(
	//					static_cast<int>(_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]->GetInfo()->TypeID),
	//					_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]->GetInfo()->ID,
	//					_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx]->GetHoldNum(),
	//					static_cast<HoldWeapon*>(_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx].get())->GetAtkRnd(),
	//					static_cast<HoldWeapon*>(_InventoryItemList[(int)Item::ItemCodeE::Weapon][idx].get())->GetMagicAtk()
	//				)));
	//		//ファイルパス
	//		char filepath3[256] = "";
	//		strcpy(filepath3, "Asset/Data/InventoryData/WeaponList.csv");
	//		Support::OutputCSV<HoldWeponInfo>(filepath3, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), work3);
	//	}
	//}
}