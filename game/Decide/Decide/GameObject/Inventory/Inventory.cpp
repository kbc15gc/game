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

	// �����F	�A�C�e����ʁB
	//			�A�C�e���ʂ��ԍ��B
	//			�������B
	//			�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	//			���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	HoldWeaponInfo::HoldWeaponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd, int CrtRnd, bool IsEquip) : HoldEquipInfo(TypeID, ID, IsEquip) {
		_AtkRnd = AtkRnd;
		_MAtkRnd = MAtkRnd;
		_CrtRnd = CrtRnd;
	}

	// �����F	�R�s�[���̃|�C���^�B
	HoldWeaponInfo::HoldWeaponInfo(HoldItemBase* info) : HoldEquipInfo(info) {
		_AtkRnd = static_cast<HoldWeapon*>(info)->GetAtkRnd();
		_MAtkRnd = static_cast<HoldWeapon*>(info)->GetMtkRnd();
		_CrtRnd = static_cast<HoldWeapon*>(info)->GetCrtRnd();
	}

	HoldArmorInfo::HoldArmorInfo(int TypeID, int ID, int Def, int MDef, bool IsEquip) : HoldEquipInfo(TypeID, ID, IsEquip) {
		_DefRnd = Def;
		_MDefRnd = MDef;
	}
	// �����F	�R�s�[���̃|�C���^�B
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

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::AddItem(Item::ItemInfo* item, int num) {
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
					_OutData(Item::ItemCodeE::Item);

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
			_OutData(Item::ItemCodeE::Item);

			return;
		}
	}


	//�G���[�񍐁B
	// ���b�菈��(�ǉ��ł��Ȃ��ꍇ�͎̂Ă�A�C�e�����v���C���[�ɑI��������K�v������)�B
	{
		char error[256];
		sprintf(error, "�C���x���g������t�Œǉ�����܂���ł����B");
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
	}

}

//�����i�̒ǉ��B
HoldEquipment* Inventory::AddEquipment(Item::BaseInfo* info, bool isRandParam) {

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((info->TypeID))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(info->TypeID)][idx] == nullptr) {
			// �󂫘g������B

			// �����i�쐬�B
			HoldEquipment* equi = nullptr;
			switch (info->TypeID)
			{
			case Item::ItemCodeE::Item:
				//�A�C�e���͕ʂ̒ǉ��֐����g���āB
				char error[256];
				sprintf(error, "�����̒ǉ���code �A�C�e�����w�肳��܂����B");
				MessageBoxA(0, error, "�����i�ǉ����s", MB_ICONWARNING);
				abort();
				break;
			case Item::ItemCodeE::Weapon:
				// ���퐶���B
				equi = INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 9);
				break;
			case Item::ItemCodeE::Armor:
				// �h����B
				equi = INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 9);
				break;
			}

			equi->SetInfo(info);
			if (isRandParam) {
				// �����_���p�����[�^�����B
				equi->CreateRandParam();
			}
			else {
				// ��l�Ńp�����[�^�����B
				equi->CreateOriginParam();
			}

			//�����i��ǉ��B
			_InventoryItemList[static_cast<int>(info->TypeID)][idx] = equi;

			//�����i�̏��������o���B
			_OutData(info->TypeID);

			return equi;
		}
	}

	//�G���[�񍐁B
	// ���b�菈��(�ǉ��ł��Ȃ��ꍇ�͎̂Ă�A�C�e�����v���C���[�ɑI��������K�v������)�B
	{
		char error[256];
		sprintf(error, "�C���x���g������t�Œǉ�����܂���ł����B");
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
	}

	return nullptr;
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
			INSTANCE(GameObjectManager)->AddRemoveList(*itr);
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

				//���X�g����폜���ꂽ�̂ő��̃A�C�e�����l�߂�B
				ArrangementInventory();
			}
			//�����i�̏����������o���B
			_OutData(item->TypeID);
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

void Inventory::_LoadData() {
	// ���R�[�h�B
	Support::DATARECORD* record[] = { Hold::ConsumptionItemData ,Hold::HoldArmorData ,Hold::HoldWeaponData };
	int recordSize[] = { ARRAY_SIZE(Hold::ConsumptionItemData),ARRAY_SIZE(Hold::HoldArmorData),ARRAY_SIZE(Hold::HoldWeaponData) };
	//�t�@�C���l�[��
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	
	FOR(i, Item::ItemCodeE::Max) {
		//�t�@�C���p�X
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);

		// �f�[�^�i�[�p�z��B
		vector<unique_ptr<Hold::HoldInfo>> work;
		//�t�@�C�����珊���i���ǂݍ��݁B
		switch (i)
		{
		case static_cast<int>(Item::ItemCodeE::Item) :
			// ����A�C�e���ǉ��B
			Support::LoadCSVData<Hold::ConsumptionInfo, Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddItem(static_cast<Item::ItemInfo*>(INSTANCE(ItemManager)->GetItemInfo(work[idx]->_ID, Item::ItemCodeE::Item)), static_cast<Hold::ConsumptionInfo*>(work[idx].get())->_HoldNum);
			}
			break;
		case static_cast<int>(Item::ItemCodeE::Armor) :
			// �h��ǉ��B
			Support::LoadCSVData<Hold::HoldArmorInfo,Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddEquipment(INSTANCE(ItemManager)->GetItemInfo(work[idx]->_ID, Item::ItemCodeE::Armor),false)->ConfigLoadData(static_cast<Hold::HoldArmorInfo*>(work[idx].get()));
			}
			break;
		case static_cast<int>(Item::ItemCodeE::Weapon) :
			// ����ǉ��B
			Support::LoadCSVData<Hold::HoldWeaponInfo, Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddEquipment(INSTANCE(ItemManager)->GetItemInfo(work[idx]->_ID, Item::ItemCodeE::Weapon), false)->ConfigLoadData(static_cast<Hold::HoldArmorInfo*>(work[idx].get()));
			}
			break;
		}
	}
}

void Inventory::_OutData(Item::ItemCodeE code) {

	//�t�@�C���l�[���B
	const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	Support::DATARECORD* recordArray[] = { Hold::ConsumptionItemData ,Hold::HoldArmorData ,Hold::HoldWeaponData };
	int recordSize[] = { ARRAY_SIZE(Hold::ConsumptionItemData),ARRAY_SIZE(Hold::HoldArmorData),ARRAY_SIZE(Hold::HoldWeaponData) };
	//CSV�ɏ����o���悤�̃��X�g�B
	vector<unique_ptr<Hold::HoldInfo>> OutPutCSVList;

	//�t�@�C���p�X�B
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
	//CSV�ɏ����o���B
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

//�C���x���g���𐮗�(ID���ɂȂ�)�B
void Inventory::SortID() {
	
	//sort(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)].begin(), _InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)].end(),IDComp);
	
}

//�C���x���g�����𐮗�(�r����null�����܂Ȃ��p�ɒ��g���l�߂邾��)�B
void Inventory::ArrangementInventory() 
{
	//�C���x���g���̒��g���ꎞ�I�Ɋi�[�����Ɨp�ϐ��B
	vector<vector<HoldItemBase*>> work= vector<vector<HoldItemBase*>>(static_cast<int>(Item::ItemCodeE::Max));

	//�C���x���g�����̃A�C�e���S�Ă�work�Ɋi�[�B
	for (int type = 0; type < static_cast<int>(Item::ItemCodeE::Max); type++)
	{
		for (auto itr = _InventoryItemList[type].begin(); itr < _InventoryItemList[type].end(); itr++)
		{
			//���g�����鎞�B
			if ((*itr) != nullptr) {

				//work�Ɋi�[�B
				work[type].push_back((*itr));

				//�i�[���I������C���x���g���͏������B
				(*itr) = nullptr;
			}
		}
	}

	//work����A�C�e�������o���ăC���x���g���ɋl�߂�B
	for (int type = 0; type < static_cast<int>(Item::ItemCodeE::Max); type++)
	{
		for (int idx = 0; idx < work[type].size(); idx++)
		{
			//�C���x���g���ɃA�C�e����ǉ��B
			_InventoryItemList[type][idx] = work[type][idx];
		}
	}
}