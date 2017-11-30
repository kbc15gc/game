#include "stdafx.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"
#include "GameObject\Inventory\Inventory.h"


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

	HoldEquipInfo::HoldEquipInfo(int TypeID, int ID, bool isEquip) :HoldInfo(TypeID, ID) {
		if (isEquip) {
			_IsEquip = 1;
		}
		else {
			_IsEquip = 0;
		}
		_Rank = HoldEquipment::Rank::C;
	}

	HoldEquipInfo::HoldEquipInfo(HoldItemBase* info) : HoldInfo(info) {
		if (static_cast<HoldEquipment*>(info)->GetIsEquip()) {
			_IsEquip = 1;
		}
		else {
			_IsEquip = 0;
		}
		_Rank = static_cast<HoldEquipment*>(info)->GetRank();
	}

	// 引数：	アイテム種別。
	//			アイテム通し番号。
	//			所持数。
	//			攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	//			魔法攻撃力の乱数差分(この値でランク付け、単位はパーセント)。
	HoldWeaponInfo::HoldWeaponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd, int CrtRnd, bool IsEquip) : HoldEquipInfo(TypeID, ID, IsEquip) {
		_AtkRnd = AtkRnd;
		_MAtkRnd = MAtkRnd;
		_DexRnd = CrtRnd;
	}

	// 引数：	コピー元のポインタ。
	HoldWeaponInfo::HoldWeaponInfo(HoldItemBase* info) : HoldEquipInfo(info) {
		_AtkRnd = static_cast<HoldWeapon*>(info)->GetAtkRnd();
		_MAtkRnd = static_cast<HoldWeapon*>(info)->GetMtkRnd();
		_DexRnd = static_cast<HoldWeapon*>(info)->GetDexRnd();
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


HoldItemBase* HoldItemFactory::CreateItem(Item::BaseInfo* info, bool isRandParam) {
	return CreateItem(info->TypeID, info->ID, isRandParam);
}

HoldItemBase* HoldItemFactory::CreateItem(Hold::HoldInfo* info) {
	HoldItemBase* item = CreateItem(static_cast<Item::ItemCodeE>(info->_TypeID), info->_ID, false);
	item->ConfigLoadData(info);
	return item;
}

HoldItemBase* HoldItemFactory::CreateItem(Item::ItemCodeE code, int id, bool isRandParam) {
	HoldItemBase* item = nullptr;
	Item::BaseInfo* info = INSTANCE(ItemManager)->GetItemInfo(id, code);
	switch (code)
	{
	case Item::ItemCodeE::Item :
		item = new ConsumptionItem("ConsumptionItem");
		item->Awake();
		INSTANCE(GameObjectManager)->Add(item, 9);
		static_cast<ConsumptionItem*>(item)->SetInfo(info);
		break;
	case Item::ItemCodeE::Armor:
		item = new HoldArmor("Armor");
		item->Awake();
		INSTANCE(GameObjectManager)->Add(item, 9);
		static_cast<HoldArmor*>(item)->SetInfo(info);
		if (isRandParam) {
			static_cast<HoldArmor*>(item)->CreateRandParam();
		}
		else {
			static_cast<HoldArmor*>(item)->CreateOriginParam();
		}
		break;
	case Item::ItemCodeE::Weapon:
		item = new HoldWeapon("Weapon");
		item->Awake();
		INSTANCE(GameObjectManager)->Add(item, 9);
		static_cast<HoldWeapon*>(item)->SetInfo(info);
		if (isRandParam) {
			static_cast<HoldWeapon*>(item)->CreateRandParam();
		}
		else {
			static_cast<HoldWeapon*>(item)->CreateOriginParam();
		}
		break;
	}

	item->Start();

	return item;
}

