#include "stdafx.h"
#include "Inventory.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"

Inventory* Inventory::_InventoryInstance = nullptr;

namespace {
	HoldInfo::HoldInfo() {

	}
	HoldInfo::HoldInfo(int TypeID, int ID) {
		_TypeID = TypeID;
		_ID = ID;
	}
	HoldInfo::HoldInfo(HoldItemBase* info) {
		_TypeID = static_cast<int>(info->GetInfo()->TypeID);
		_ID = info->GetInfo()->ID;
	}


	ConsumptionInfo::ConsumptionInfo(int TypeID, int ID, int num) {
	}

	ConsumptionInfo::ConsumptionInfo(HoldItemBase* info) :HoldInfo(info) {
		_HoldNum = static_cast<ConsumptionItem*>(info)->GetHoldNum();
	}

	HoldEquipInfo::HoldEquipInfo(int TypeID, int ID, bool isEquip) :HoldInfo(TypeID,ID){
		_IsEquip = isEquip;
	}

	HoldEquipInfo::HoldEquipInfo(HoldItemBase* info) : HoldInfo(info) {
		_IsEquip = static_cast<HoldEquipment*>(info)->GetIsEquip();
	}

	// 引数：	アイテム種別。
	//			アイテム通し番号。
	//			所持数。
	//			攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	//			魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	HoldWeaponInfo::HoldWeaponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd, int CrtRnd, bool IsEquip) : HoldEquipInfo(TypeID, ID, IsEquip) {
		_AtkRnd = AtkRnd;
		_MAtkRnd = MAtkRnd;
		_CrtRnd = CrtRnd;
		_IsEquip = IsEquip;
	}

	// 引数：	コピー元のポインタ。
	HoldWeaponInfo::HoldWeaponInfo(HoldItemBase* info) : HoldEquipInfo(info) {
		_AtkRnd = static_cast<HoldWeapon*>(info)->GetAtkRnd();
		_MAtkRnd = static_cast<HoldWeapon*>(info)->GetMtkRnd();
		_CrtRnd = static_cast<HoldWeapon*>(info)->GetCrtRnd();
	}

	HoldArmorInfo::HoldArmorInfo(int TypeID, int ID, int Def, int MDef, bool IsEquip) : HoldEquipInfo(TypeID, ID, IsEquip) {
		_DefRnd = Def;
		_MDefRnd = MDef;
	}
	// 引数：	コピー元のポインタ。
	HoldArmorInfo::HoldArmorInfo(HoldItemBase* info) : HoldEquipInfo(info) {
		_DefRnd = static_cast<HoldArmor*>(info)->GetDefRnd();
		_MDefRnd = static_cast<HoldArmor*>(info)->GetMDefRnd();
	}

}


Inventory::Inventory()
{
	_InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(Item::ItemCodeE::Max), vector<HoldItemBase*>(INVENTORYLISTNUM, nullptr));
}

void Inventory::Initialize() {
	
	// レコード。
	Support::DATARECORD* record[] = { ConsumptionItemData ,HoldArmorData ,HoldWeaponData };
	//ファイルネーム。
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	// データ格納用配列。
	vector<vector<unique_ptr<HoldInfo>>> work= vector<vector<unique_ptr<HoldInfo>>>(static_cast<int>(Item::ItemCodeE::Max));

	FOR(i, Item::ItemCodeE::Max) {
		//ファイルパス
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);

		//ファイルから所持品情報読み込み。
		Support::LoadCSVData<HoldInfo>(filepath, record[i], ARRAY_SIZE(record[i]), work[i]);
		// 読み込んだ情報から所持品を作成。
		for (int idx = 0; idx < work[i].size(); idx++)
		{
			switch (i)
			{
			case static_cast<int>(Item::ItemCodeE::Item) :
				// 消費アイテム追加。
				AddItem(static_cast<Item::ItemInfo*>(INSTANCE(ItemManager)->GetItemInfo(work[i][idx]->_ID, Item::ItemCodeE::Item)),static_cast<ConsumptionInfo*>(work[i][idx].get())->_HoldNum);
				break;
			case static_cast<int>(Item::ItemCodeE::Armor) :
				// 防具追加。
				AddEquipment(INSTANCE(GameObjectManager)->AddNew<HoldArmor>("Armor", 9), Item::ItemCodeE::Armor)->ConfigLoadData(static_cast<HoldArmorInfo*>(work[i][idx].get()));
				break;
			case static_cast<int>(Item::ItemCodeE::Weapon) :
				// 武器追加。
				AddEquipment(INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("Weapon", 9), Item::ItemCodeE::Weapon)->ConfigLoadData(static_cast<HoldWeaponInfo*>(work[i][idx].get()));
				break;
			}
		}
	}
}

//アイテムをインベントリに追加。
void Inventory::AddItem(Item::ItemInfo* item, int num) {
	Item::BaseInfo* Info = item;
	char error[256];

	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] && _InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]->GetInfo()->ID == item->ID)
		{
			//同じアイテムが配列にある。

			int holdNum = static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->GetHoldNum();
			if (holdNum < Item::holdMax) {
				// このアイテムの所持数に空きがある。

				if (holdNum + num > Item::holdMax) {
					// 加算すると所持上限を超える。

					int set = Item::holdMax - holdNum;	// 加算する数。

					//所持数更新。
					static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->UpdateHoldNum(set);

					num -= set;	// 設定した分は減算。
				}
				else {
					// 追加した分の数が一枠に収まる。

					// アイテムの所持数を加算。
					static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->UpdateHoldNum(num);

					//所持品配列の所持数をCSVに書き出し。
					_ItemListOutData();

					return;
				}
			}
		}
	}

	// 追加するアイテムが配列になかった。
	// もしくは既に存在するアイテム枠に追加分の数が収まりきらなかった。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] == nullptr) {
			//インベントリに空きがある。

			//所持アイテムのインスタンス作成。
			ConsumptionItem* Hold = INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("Consumption", 9);
			Hold->SetInfo(item);
			//所持数更新。
			static_cast<ConsumptionItem*>(Hold)->UpdateHoldNum(num);

			// 新しく作成したアイテムを配列に追加。
			_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] = Hold;

			//所持品配列の所持数をCSVに書き出し。
			_ItemListOutData();

			return;
		}
	}



	{
		//エラー報告。
		// ※暫定処理(追加できない場合は捨てるアイテムをプレイヤーに選択させる必要がある)。
		char error[256];
		sprintf(error, "インベントリが一杯で追加されませんでした。");
		MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
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
			return _InventoryItemList[static_cast<int>(code)][i];
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
		if (item != *itr) {
			itr++;
		}
		else
		{
			//一致したので中身を削除。
			(*itr) = nullptr;
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
			static_cast<ConsumptionItem*>(*itr)->UpdateHoldNum(-num);

			//更新した結果所持数が0になれば破棄。
			if (static_cast<ConsumptionItem*>(*itr)->GetHoldNum() <= 0) {
				//リストから削除。
				_DeleteFromList(*itr);
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
	Support::DATARECORD* recordArray[] = { ConsumptionItemData ,HoldArmorData ,HoldWeaponData };

	//CSVに書き出すようのリスト。
	vector<vector<unique_ptr<HoldInfo>>> OutPutCSVList = vector<vector<unique_ptr<HoldInfo>>>(static_cast<int>(Item::ItemCodeE::Max));

	FOR(i, Item::ItemCodeE::Max)
	{
		//ファイルパス。
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);

		for (int idx = 0; idx < _InventoryItemList[i].size(); idx++) {
			unique_ptr<HoldInfo> ptr;
			if (_InventoryItemList[i][idx]) {
				switch (i) {
				case static_cast<int>(Item::ItemCodeE::Item) :
					ptr = unique_ptr<HoldInfo>(new ConsumptionInfo(_InventoryItemList[i][idx]));
					break;
				case static_cast<int>(Item::ItemCodeE::Armor) :
					ptr = unique_ptr<HoldInfo>(new HoldArmorInfo(_InventoryItemList[i][idx]));
					break;
				case static_cast<int>(Item::ItemCodeE::Weapon) :
					ptr = unique_ptr<HoldInfo>(new HoldWeponInfo(_InventoryItemList[i][idx]));
					break;
				}
				//持っているアイテムの情報を書き出し用のリストに追加。
				OutPutCSVList[i].push_back(move(ptr));
			}
		}
		//ItemList.CSVに書き出し。
		Support::OutputCSV<HoldInfo>(filepath, recordArray[i], ARRAY_SIZE(recordArray[i]), OutPutCSVList[i]);
	}
}

//装備品の追加。
HoldEquipment* Inventory::AddEquipment(HoldEquipment* equi, Item::ItemCodeE code) {
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
			_InventoryItemList[static_cast<int>(code)][idx] = equi;

			//所持品の情報を書き出し。
			_ItemListOutData();

			return equi;
		}
	}	

	return nullptr;
}

void Inventory::SortID() {

}