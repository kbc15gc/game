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

	// �����F	�A�C�e����ʁB
	//			�A�C�e���ʂ��ԍ��B
	//			�������B
	//			�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	//			���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	HoldWeponInfo::HoldWeponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd, int CrtRnd, bool IsEquip) : HoldInfo(TypeID, ID) {
		_AtkRnd = AtkRnd;
		_MAtkRnd = MAtkRnd;
		_CrtRnd = CrtRnd;
		_IsEquip = IsEquip;
	}

	// �����F	�R�s�[���̃|�C���^�B
	HoldWeponInfo::HoldWeponInfo(HoldItemBase* info) : HoldInfo(info) {
		_AtkRnd = static_cast<HoldWeapon*>(info)->GetAtkRnd();
		_MAtkRnd = static_cast<HoldWeapon*>(info)->GetMtkRnd();
		_CrtRnd = static_cast<HoldWeapon*>(info)->GetCrtRnd();
		_IsEquip = static_cast<HoldWeapon*>(info)->GetIsEquip();
	}

	HoldArmorInfo::HoldArmorInfo(int TypeID, int ID, int Def, int MDef, bool IsEquip) : HoldInfo(TypeID, ID) {
		_DefRnd = Def;
		_MDefRnd = MDef;
		_IsEquip = IsEquip;
	}
	// �����F	�R�s�[���̃|�C���^�B
	HoldArmorInfo::HoldArmorInfo(HoldItemBase* info) : HoldInfo(info) {
		_DefRnd = static_cast<HoldArmor*>(info)->GetDefRnd();
		_MDefRnd = static_cast<HoldArmor*>(info)->GetMDefRnd();
		_IsEquip = static_cast<HoldArmor*>(info)->GetIsEqip();
	}

}


Inventory::Inventory()
{
	_InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(Item::ItemCodeE::Max), vector<HoldItemBase*>(INVENTORYLISTNUM, nullptr));
}

void Inventory::Initialize() {
	
	AddItem(INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Item));
	AddItem(INSTANCE(ItemManager)->GetItemInfo(1, Item::ItemCodeE::Item));

	//�t�@�C���l�[��
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	vector<vector<unique_ptr<HoldInfo>>> work= vector<vector<unique_ptr<HoldInfo>>>(static_cast<int>(Item::ItemCodeE::Max));
	FOR(i, ARRAY_SIZE(filename)) {
		//�t�@�C���p�X
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);
		switch (i)
		{
		case static_cast<int>((Item::ItemCodeE::Item)):
			//�t�@�C������A�C�e�����ǂݍ��݁B
			//Support::LoadCSVData<HoldInfo>(filepath, HoldItemData, ARRAY_SIZE(HoldItemData), work[static_cast<int>((Item::ItemCodeE::Item))]);
			/*for (int idx = 0; idx < work[static_cast<int>((Item::ItemCodeE::Item))].size(); idx++)
			{
			}*/
			break;
		case static_cast<int>((Item::ItemCodeE::Armor)):
			//�t�@�C������h����ǂݍ��݁B
			//Support::LoadCSVData<HoldInfo>(filepath, HoldArmorData, ARRAY_SIZE(HoldArmorData), work[static_cast<int>((Item::ItemCodeE::Armor))]);
			/*for (int idx = 0; idx < work[static_cast<int>((Item::ItemCodeE::Armor))].size(); idx++)
			{
			}*/
			break;
		case static_cast<int>((Item::ItemCodeE::Weapon)):
			//�t�@�C�����畐����ǂݍ��݁B
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

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::AddItem(Item::BaseInfo* item, int num) {
	Item::BaseInfo* Info = item;
	char error[256];

	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] && _InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]->GetInfo()->ID == item->ID)
		{
			//�����A�C�e�����z��ɂ���B

			int holdNum = static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->GetHoldNum();
			if (holdNum < Item::holdMax) {
				// ���̃A�C�e���̏������ɋ󂫂�����B

				if (holdNum + num > Item::holdMax) {
					// ���Z����Ə�������𒴂���B

					int set = Item::holdMax - holdNum;	// ���Z���鐔�B

					//�������X�V�B
					static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->UpdateHoldNum(set);

					num -= set;	// �ݒ肵�����͌��Z�B
				}
				else {
					// �ǉ��������̐�����g�Ɏ��܂�B

					// �A�C�e���̏����������Z�B
					static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->UpdateHoldNum(num);

					//�����i�z��̏�������CSV�ɏ����o���B
					_ItemListOutData();

					return;
				}
			}
		}
	}

	// �ǉ�����A�C�e�����z��ɂȂ������B
	// �������͊��ɑ��݂���A�C�e���g�ɒǉ����̐������܂肫��Ȃ������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] == nullptr) {
			//�C���x���g���ɋ󂫂�����B

			//�����A�C�e���̃C���X�^���X�쐬�B
			ConsumptionItem* Hold = INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("Consumption", 9);
			Hold->SetInfo(item);
			//�������X�V�B
			static_cast<ConsumptionItem*>(Hold)->UpdateHoldNum(num);

			// �V�����쐬�����A�C�e����z��ɒǉ��B
			_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] = Hold;

			//�����i�z��̏�������CSV�ɏ����o���B
			_ItemListOutData();

			return;
		}
	}



	{
		//�G���[�񍐁B
		// ���b�菈��(�ǉ��ł��Ȃ��ꍇ�͎̂Ă�A�C�e�����v���C���[�ɑI��������K�v������)�B
		char error[256];
		sprintf(error, "�C���x���g������t�Œǉ�����܂���ł����B");
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
	}

}

void Inventory::UseItem() {
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][_NowLookItemPos];
}

//�A�C�e���R�[�h��ID�����ɔz�񂩂猟���B
HoldItemBase* Inventory::FindItem(Item::ItemCodeE code, const unsigned int& id) {

	//�z��T�C�Y�������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//�����B
		if (_InventoryItemList[static_cast<int>(code)][i] && _InventoryItemList[static_cast<int>(code)][i]->GetInfo()->ID == id) {
			return _InventoryItemList[static_cast<int>(code)][i];
		}
	}

	//������Ȃ������B
	char error[256];
	sprintf(error, "�w�肳�ꂽ�A�C�e����������܂���ł����B");
	MessageBoxA(0, error, "�A�C�e���̌������s", MB_ICONWARNING);
}

//���X�g����w�肳�ꂽ�A�C�e�����폜�B
void Inventory::_DeleteFromList(HoldItemBase* item) {

	//�z��T�C�Y�������B
	for (auto itr = _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)].begin() ; itr != _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)].end();)
	{
		//���g�������܂��͕s��v�B
		if (item != *itr) {
			itr++;
		}
		else
		{
			//��v�����̂Œ��g���폜�B
			(*itr) = nullptr;
			return;
		}
	}
}

//�����������炷�B
bool Inventory::SubHoldNum(Item::BaseInfo* item, int num) {
	//�z��T�C�Y�������B
	for (auto itr = _InventoryItemList[static_cast<int>(item->TypeID)].begin(); itr != _InventoryItemList[static_cast<int>(item->TypeID)].end();)
	{
		//ID�̈�v�B
		if ((*itr) != nullptr&&item->ID == (*itr)->GetInfo()->ID)
		{
			//�����������i�̐����X�V�B
			static_cast<ConsumptionItem*>(*itr)->UpdateHoldNum(-num);

			//�X�V�������ʏ�������0�ɂȂ�Δj���B
			if (static_cast<ConsumptionItem*>(*itr)->GetHoldNum() <= 0) {
				//���X�g����폜�B
				_DeleteFromList(*itr);
			}
			//�����i�̏����������o���B
			_ItemListOutData();
			return true;
		}
		//�s��v�B
		else
		{
			itr++;
		}
	}
	return false;
}

void Inventory::_ItemListOutData() {

	//�t�@�C���l�[���B
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	Support::DATARECORD* recordArray[] = { ConsumptionItemData ,HoldArmorData ,HoldWeaponData };

	//CSV�ɏ����o���悤�̃��X�g�B
	vector<vector<unique_ptr<HoldInfo>>> OutPutCSVList = vector<vector<unique_ptr<HoldInfo>>>(static_cast<int>(Item::ItemCodeE::Max));

	FOR(i, Item::ItemCodeE::Max)
	{
		//�t�@�C���p�X�B
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
				//�����Ă���A�C�e���̏��������o���p�̃��X�g�ɒǉ��B
				OutPutCSVList[i].push_back(move(ptr));
			}
		}
		//ItemList.CSV�ɏ����o���B
		Support::OutputCSV<HoldInfo>(filepath, recordArray[i], ARRAY_SIZE(recordArray[i]), OutPutCSVList[i]);
	}
}

//�����i�̒ǉ��B
void Inventory::AddEquipment(HoldEquipment* equi, Item::ItemCodeE code) {
	if (code == Item::ItemCodeE::Item) {
		//�A�C�e���͕ʂ̒ǉ��֐����g���āB
		char error[256];
		sprintf(error, "�����̒ǉ���code �A�C�e�����w�肳��܂����B");
		MessageBoxA(0, error, "�����i�ǉ����s", MB_ICONWARNING);
		abort();
	}

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((code))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(code)][idx] == nullptr) {

			//�����i��ǉ��B
			_InventoryItemList[static_cast<int>(code)][idx] = equi;

			//�����i�̏��������o���B
			_ItemListOutData();
			break;
		}
	}	
}

void Inventory::SortID() {

}