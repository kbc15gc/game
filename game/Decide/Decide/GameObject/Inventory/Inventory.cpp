#include "stdafx.h"
#include "Inventory.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\ItemManager\HoldItem\HoldEquipment.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"

Inventory* Inventory::_InventoryInstance = nullptr;

namespace Hold {
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

	HoldEquipInfo::HoldEquipInfo() {

	}

	HoldEquipInfo::HoldEquipInfo(int TypeID, int ID, bool isEquip) :HoldInfo(TypeID,ID){
		if (isEquip) {
			_IsEquip = 1;
		}
		else {
			_IsEquip = 0;
		}
	}

	HoldEquipInfo::HoldEquipInfo(HoldItemBase* info) : HoldInfo(info) {
		if (static_cast<HoldEquipment*>(info)->GetIsEquip()) {
			_IsEquip = 1;
		}
		else {
			_IsEquip = 0;
		}
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

};


Inventory::Inventory()
{
	_InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(Item::ItemCodeE::Max), vector<HoldItemBase*>(INVENTORYLISTNUM, nullptr));
}

Inventory::~Inventory(){
	for (auto& list : _InventoryItemList) {
		for (auto p : list) {
			INSTANCE(GameObjectManager)->AddRemoveList(p);
		}
	}
}

void Inventory::Initialize() {
	
	_LoadData();

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
					_OutData(Item::ItemCodeE::Item);

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
			_OutData(Item::ItemCodeE::Item);

			return;
		}
	}


	//エラー報告。
	// ※暫定処理(追加できない場合は捨てるアイテムをプレイヤーに選択させる必要がある)。
	{
		char error[256];
		sprintf(error, "インベントリが一杯で追加されませんでした。");
		MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
	}

}

//装備品の追加。
HoldEquipment* Inventory::AddEquipment(Item::BaseInfo* info, bool isRandParam) {

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((info->TypeID))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(info->TypeID)][idx] == nullptr) {
			// 空き枠がある。

			// 装備品作成。
			HoldEquipment* equi = nullptr;
			switch (info->TypeID)
			{
			case Item::ItemCodeE::Item:
				//アイテムは別の追加関数を使って。
				char error[256];
				sprintf(error, "装備の追加でcode アイテムが指定されました。");
				MessageBoxA(0, error, "装備品追加失敗", MB_ICONWARNING);
				abort();
				break;
			case Item::ItemCodeE::Weapon:
				// 武器生成。
				equi = INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 9);
				break;
			case Item::ItemCodeE::Armor:
				// 防具生成。
				equi = INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 9);
				break;
			}

			equi->SetInfo(info);
			if (isRandParam) {
				// ランダムパラメータ生成。
				equi->CreateRandParam();
			}
			else {
				// 基準値でパラメータ生成。
				equi->CreateOriginParam();
			}

			//装備品を追加。
			_InventoryItemList[static_cast<int>(info->TypeID)][idx] = equi;

			//装備品の情報を書き出し。
			_OutData(info->TypeID);

			return equi;
		}
	}

	//エラー報告。
	// ※暫定処理(追加できない場合は捨てるアイテムをプレイヤーに選択させる必要がある)。
	{
		char error[256];
		sprintf(error, "インベントリが一杯で追加されませんでした。");
		MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
	}

	return nullptr;
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
			INSTANCE(GameObjectManager)->AddRemoveList(*itr);
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

				//リストから削除されたので他のアイテムを詰める。
				ArrangementInventory();
			}
			//所持品の所持数書き出し。
			_OutData(item->TypeID);
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

void Inventory::_LoadData() {
	// レコード。
	Support::DATARECORD* record[] = { Hold::ConsumptionItemData ,Hold::HoldArmorData ,Hold::HoldWeaponData };
	int recordSize[] = { ARRAY_SIZE(Hold::ConsumptionItemData),ARRAY_SIZE(Hold::HoldArmorData),ARRAY_SIZE(Hold::HoldWeaponData) };
	//ファイルネーム
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	
	FOR(i, Item::ItemCodeE::Max) {
		//ファイルパス
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);

		// データ格納用配列。
		vector<unique_ptr<Hold::HoldInfo>> work;
		//ファイルから所持品情報読み込み。
		switch (i)
		{
		case static_cast<int>(Item::ItemCodeE::Item) :
			// 消費アイテム追加。
			Support::LoadCSVData<Hold::ConsumptionInfo, Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddItem(static_cast<Item::ItemInfo*>(INSTANCE(ItemManager)->GetItemInfo(work[idx]->_ID, Item::ItemCodeE::Item)), static_cast<Hold::ConsumptionInfo*>(work[idx].get())->_HoldNum);
			}
			break;
		case static_cast<int>(Item::ItemCodeE::Armor) :
			// 防具追加。
			Support::LoadCSVData<Hold::HoldArmorInfo,Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddEquipment(INSTANCE(ItemManager)->GetItemInfo(work[idx]->_ID, Item::ItemCodeE::Armor),false)->ConfigLoadData(static_cast<Hold::HoldArmorInfo*>(work[idx].get()));
			}
			break;
		case static_cast<int>(Item::ItemCodeE::Weapon) :
			// 武器追加。
			Support::LoadCSVData<Hold::HoldWeaponInfo, Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddEquipment(INSTANCE(ItemManager)->GetItemInfo(work[idx]->_ID, Item::ItemCodeE::Weapon), false)->ConfigLoadData(static_cast<Hold::HoldArmorInfo*>(work[idx].get()));
			}
			break;
		}
	}
}

void Inventory::_OutData(Item::ItemCodeE code) {

	//ファイルネーム。
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	Support::DATARECORD* recordArray[] = { Hold::ConsumptionItemData ,Hold::HoldArmorData ,Hold::HoldWeaponData };
	int recordSize[] = { ARRAY_SIZE(Hold::ConsumptionItemData),ARRAY_SIZE(Hold::HoldArmorData),ARRAY_SIZE(Hold::HoldWeaponData) };
	//CSVに書き出すようのリスト。
	vector<unique_ptr<Hold::HoldInfo>> OutPutCSVList;

	//ファイルパス。
	char filepath[256] = "";
	sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[static_cast<int>(code)]);

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>(code)].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(code)][idx]) {
			switch (code) {
			case Item::ItemCodeE::Item:
				OutPutCSVList.push_back(unique_ptr<Hold::HoldInfo>(new Hold::ConsumptionInfo(_InventoryItemList[static_cast<int>(code)][idx])));
				break;
			case Item::ItemCodeE::Armor:
				OutPutCSVList.push_back(unique_ptr<Hold::HoldInfo>(new Hold::HoldArmorInfo(_InventoryItemList[static_cast<int>(code)][idx])));
				break;
			case Item::ItemCodeE::Weapon:
				OutPutCSVList.push_back(unique_ptr<Hold::HoldInfo>(new Hold::HoldWeaponInfo(_InventoryItemList[static_cast<int>(code)][idx])));
				break;
			}
		}
	}
	//CSVに書き出し。
	Support::OutputCSV<Hold::HoldInfo>(filepath, recordArray[static_cast<int>(code)], recordSize[static_cast<int>(code)], OutPutCSVList);
}

void Inventory::_OutData_All() {
	FOR(i, Item::ItemCodeE::Max)
	{
		_OutData(static_cast<Item::ItemCodeE>(i));
	}
}

//bool Inventory::IDComp(HoldItemBase* left,HoldItemBase* rigth)
//{
//	//return left->GetInfo()->ID > rigth->GetInfo()->ID;
//}

//インベントリを整列(ID順になる)。
void Inventory::SortID() {
	
	//sort(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)].begin(), _InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)].end(),IDComp);
	
}

//インベントリ内を整理(途中にnullを挟まない用に中身を詰めるだけ)。
void Inventory::ArrangementInventory() 
{
	//インベントリの中身を一時的に格納する作業用変数。
	vector<vector<HoldItemBase*>> work= vector<vector<HoldItemBase*>>(static_cast<int>(Item::ItemCodeE::Max));

	//インベントリ内のアイテム全てをworkに格納。
	for (int type = 0; type < static_cast<int>(Item::ItemCodeE::Max); type++)
	{
		for (auto itr = _InventoryItemList[type].begin(); itr < _InventoryItemList[type].end(); itr++)
		{
			//中身がある時。
			if ((*itr) != nullptr) {

				//workに格納。
				work[type].push_back((*itr));

				//格納し終わったインベントリは初期化。
				(*itr) = nullptr;
			}
		}
	}

	//workからアイテムを取り出してインベントリに詰める。
	for (int type = 0; type < static_cast<int>(Item::ItemCodeE::Max); type++)
	{
		for (int idx = 0; idx < work[type].size(); idx++)
		{
			//インベントリにアイテムを追加。
			_InventoryItemList[type][idx] = work[type][idx];
		}
	}
}