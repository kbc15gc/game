#include "stdafx.h"
#include "Inventory.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\ItemManager\HoldItem\HoldEquipment.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\TextImage\BackWindowAndAttentionText.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

Inventory::~Inventory(){
	for (auto& list : _InventoryItemList) {
		for (auto p : list) {
			INSTANCE(GameObjectManager)->AddRemoveList(p);
		}
	}
}

void Inventory::Initialize() {
	_InventoryItemList.clear();
	_InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(Item::ItemCodeE::Max), vector<HoldItemBase*>(INVENTORYLISTNUM, nullptr));
	
	if (_HoldNumList.size() > 0) {
		_HoldNumList.clear();
	}

	for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++) {
		// コードごとの最大ID数分配列確保。
		_HoldNumList.push_back(vector<int>(INSTANCE(ItemManager)->GetMaxID(static_cast<Item::ItemCodeE>(idx)) + 1, 0));
	}

	_PlayerMoney = 100;

	if (IS_CONTINUE)
	{
		// CSV読み込み。
		_LoadData();
	}
	else
	{
		SaveMoney();
		_OutData_All();
	}
}

//アイテムをインベントリに追加。
bool Inventory::AddItem(Item::ItemInfo* item, int num) {
	
	Item::BaseInfo* Info = item;
	char error[256];
	int work = num;

	vector<vector<HoldItemBase*>::iterator> nullList;	// インベントリの空き。

	for (int i = 0;work > 0 && i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]) {
			if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]->GetInfo()->ID == item->ID)
			{
				//同じアイテムが配列にある。

				//所持数更新。
				work = static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->AddHoldNum(work);
			}
		}
		else {
			// 空の領域を保存。
			nullList.push_back(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)].begin() + i);
		}
	}

	// 追加するアイテムが配列になかった。
	// もしくは既に存在するアイテム枠に追加分の数が収まりきらなかった。
	for (auto itr = nullList.begin(); work > 0 && itr != nullList.end();)
	{
		**itr = HoldItemFactory::CreateItem(item,false);
		work = static_cast<ConsumptionItem*>(**itr)->SetHoldNum(work);
		itr = nullList.erase(itr);
	}

	nullList.clear();

	_HoldNumList[static_cast<int>(Item::ItemCodeE::Item)][Info->ID] += (num - work);	// 新しく追加した数だけ所持数合計に加算。
	//所持品配列の所持数をCSVに書き出し。
	_OutData(Item::ItemCodeE::Item);


	//エラー報告。
	// ※暫定処理(追加できない場合は捨てるアイテムをプレイヤーに選択させる必要がある)。
	if(work > 0){
		wchar_t ErrorText[256];
		wcscpy_s(ErrorText, wcslen(L"所持品が一杯でアイテムを追加出来ませんでした。\n所持品を整理してください。Aボタンで閉じる。") + 1, L"所持品が一杯でアイテムを追加出来ませんでした。\n所持品を整理してください。Aボタンで閉じる。");
		static_cast<BackWindowAndAttentionText*>(INSTANCE(GameObjectManager)->FindObject("BackWindowAndAttentionText"))->Create(ErrorText,Vector3(642.0f, 363.0f, 0.0f), Vector2(793.0f, 229.0f), Vector3(1.0f, -20.0f, 0.0), 33.0f, Color::red);
		return false;
	}

	return true;
}

//装備品の追加。
HoldEquipment* Inventory::AddEquipment(Item::BaseInfo* info, bool isRandParam) {

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((info->TypeID))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(info->TypeID)][idx] == nullptr) {
			// 空き枠がある。

			// 装備品作成。
			HoldItemBase* equi = nullptr;
			if (info->TypeID != Item::ItemCodeE::Item) {
				equi = HoldItemFactory::CreateItem(info,isRandParam);
				//装備品を追加。
				_InventoryItemList[static_cast<int>(info->TypeID)][idx] = equi;
				_HoldNumList[static_cast<int>(info->TypeID)][info->ID] += 1;	// 新しく追加した数だけ所持数合計に加算。
				//装備品の情報を書き出し。
				_OutData(info->TypeID);
				return static_cast<HoldEquipment*>(equi);;
			}
#ifdef _DEBUG
			else {
				//アイテムは別の追加関数を使って。
				char error[256];
				sprintf(error, "装備の追加でcode アイテムが指定されました。");
				MessageBoxA(0, error, "装備品追加失敗", MB_ICONWARNING);
				abort();
			}
#endif
		}
	}

	//エラー報告。
	// ※暫定処理(追加できない場合は捨てるアイテムをプレイヤーに選択させる必要がある)。
	{
		wchar_t ErrorText[256];
		wcscpy_s(ErrorText, wcslen(L"装備品が一杯で装備品を追加出来ませんでした。\n装備品を整理してください。Aボタンで閉じる。") + 1, L"装備品が一杯で装備品を追加出来ませんでした。\n装備品を整理してください。Aボタンで閉じる。");
		static_cast<BackWindowAndAttentionText*>(INSTANCE(GameObjectManager)->FindObject("BackWindowAndAttentionText"))->Create(ErrorText, Vector3(642.0f, 363.0f, 0.0f), Vector2(793.0f, 229.0f), Vector3(1.0f, -20.0f, 0.0), 33.0f, Color::red);
		return nullptr;
	}
}

bool Inventory::AddEquipment(HoldEquipment* add) {

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((add->GetInfo()->TypeID))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(add->GetInfo()->TypeID)][idx] == nullptr) {
			// 空き枠がある。

			//装備品を追加。
			_InventoryItemList[static_cast<int>(add->GetInfo()->TypeID)][idx] = add;

			_HoldNumList[static_cast<int>(add->GetInfo()->TypeID)][add->GetInfo()->ID] += 1;	// 新しく追加した数だけ所持数合計に加算。
			//装備品の情報を書き出し。
			_OutData(add->GetInfo()->TypeID);
			return true;
		}
	}

	//エラー報告。
	// ※暫定処理(追加できない場合は捨てるアイテムをプレイヤーに選択させる必要がある)。
	{
		return false;
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
	for (int idx = 0;idx < _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)].size();idx++)
	{
		//中身が無いまたは不一致。
		if (_InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)][idx] && item == _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)][idx]) {
			//武具を捨てようとしている。
			if (item->GetInfo()->TypeID==Item::ItemCodeE::Armor || item->GetInfo()->TypeID == Item::ItemCodeE::Weapon) {
				//装備している武具を捨てようとしている。
				if (static_cast<HoldEquipment*>(item)->GetIsEquip() == true) {
					return;
				}
			}
			//一致したので中身を削除。
			INSTANCE(GameObjectManager)->AddRemoveList(_InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)][idx]);
			_InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)][idx] = nullptr;
			return;
		}
	}
}

//所持数を減らす。
Inventory::SubHoldReturnValueE Inventory::SubHoldNum(HoldItemBase* item, int num) {
	//戻り値。
	SubHoldReturnValueE ret = SubHoldReturnValueE::Error;

	if (num <= 0)
		return ret;

	//装備しているなら減らさない。
	if (item->GetInfo()->TypeID == Item::ItemCodeE::Armor || item->GetInfo()->TypeID == Item::ItemCodeE::Weapon) {
		if (static_cast<HoldEquipment*>(item)->GetIsEquip() == true) {
			return ret;
		}
	}

	int work = num;

	//配列サイズ分検索。
	for (auto inv : _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)])
	{
		//IDの一致。
		if (inv != nullptr && item == inv)
		{
			ret = SubHoldReturnValueE::Sub;
			switch (item->GetInfo()->TypeID) {
			case Item::ItemCodeE::Item:
				//引数分所持品の数を更新。
				work = static_cast<ConsumptionItem*>(inv)->SubHoldNum(work);
				//更新した結果所持数が0になれば破棄。
				if (static_cast<ConsumptionItem*>(inv)->GetHoldNum() <= 0) {
					//リストから削除。
					_DeleteFromList(inv);
					//リストから削除されたので他のアイテムを詰める。
					ArrangementInventory();
					ret = SubHoldReturnValueE::Delete;
				}
				break;
			case Item::ItemCodeE::Armor:
			case Item::ItemCodeE::Weapon:
				_DeleteFromList(inv);
				work--;
				//リストから削除されたので他のアイテムを詰める。
				ArrangementInventory();
				ret = SubHoldReturnValueE::Delete;
				break;
			}
			break;
		}
	}

	_HoldNumList[static_cast<int>(item->GetInfo()->TypeID)][item->GetInfo()->ID] -= (num - work);	// 減らした数だけ所持数合計を減算。

	//所持品の所持数書き出し。
	_OutData(item->GetInfo()->TypeID);

	return ret;
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
		sprintf(filepath, "C:\\KBCGame\\ReHistory\\SaveData\\%s.csv", filename[i]);

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
				AddEquipment(static_cast<HoldEquipment*>(HoldItemFactory::CreateItem(work[idx].get())));
			}
			break;
		case static_cast<int>(Item::ItemCodeE::Weapon) :
			// 武器追加。
			Support::LoadCSVData<Hold::HoldWeaponInfo, Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddEquipment(static_cast<HoldEquipment*>(HoldItemFactory::CreateItem(work[idx].get())));
			}
			break;
		}
	}

	JsonData MoneyData;
	if (MoneyData.Load("Money"))
	{
		picojson::object money = MoneyData.GetDataObject("Money");
		_PlayerMoney = money["Money"].get<double>();
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
	sprintf(filepath, "C:\\KBCGame\\ReHistory\\SaveData\\%s.csv", filename[static_cast<int>(code)]);

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

//リストの中身を全て削除。
void Inventory::deleteList() {
	

	for (auto& list : _InventoryItemList) {
		list.clear();
	}
	_InventoryItemList.clear();
}
