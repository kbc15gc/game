#include "stdafx.h"
#include "Inventory.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\ItemManager\HoldItem\HoldEquipment.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"

Inventory* Inventory::_InventoryInstance = nullptr;


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

	for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++) {
		// �R�[�h���Ƃ̍ő�ID�����z��m�ہB
		_HoldNumList.push_back(vector<int>(INSTANCE(ItemManager)->GetMaxID(static_cast<Item::ItemCodeE>(idx)) + 1,0));
	}

	// CSV�ǂݍ��݁B
	_LoadData();
}

//�A�C�e�����C���x���g���ɒǉ��B
bool Inventory::AddItem(Item::ItemInfo* item, int num) {
	Item::BaseInfo* Info = item;
	char error[256];
	int work = num;

	vector<vector<HoldItemBase*>::iterator> nullList;	// �C���x���g���̋󂫁B

	for (int i = 0;work > 0 && i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]) {
			if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]->GetInfo()->ID == item->ID)
			{
				//�����A�C�e�����z��ɂ���B

				//�������X�V�B
				work = static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i])->AddHoldNum(work);
			}
		}
		else {
			// ��̗̈��ۑ��B
			nullList.push_back(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)].begin() + i);
		}
	}

	// �ǉ�����A�C�e�����z��ɂȂ������B
	// �������͊��ɑ��݂���A�C�e���g�ɒǉ����̐������܂肫��Ȃ������B
	for (auto itr = nullList.begin(); work > 0 && itr != nullList.end();)
	{
		**itr = HoldItemFactory::CreateItem(item,false);
		work = static_cast<ConsumptionItem*>(**itr)->SetHoldNum(work);
		itr = nullList.erase(itr);
	}

	nullList.clear();

	_HoldNumList[static_cast<int>(Item::ItemCodeE::Item)][Info->ID] += (num - work);	// �V�����ǉ��������������������v�ɉ��Z�B
	//�����i�z��̏�������CSV�ɏ����o���B
	_OutData(Item::ItemCodeE::Item);


	//�G���[�񍐁B
	// ���b�菈��(�ǉ��ł��Ȃ��ꍇ�͎̂Ă�A�C�e�����v���C���[�ɑI��������K�v������)�B
	if(work > 0){
		char error[256];
		sprintf(error, "�C���x���g������t��%s��%d���ǉ�����܂���ł����B",Info->Name,work);
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
		return false;
	}

	return true;
}

//�����i�̒ǉ��B
HoldEquipment* Inventory::AddEquipment(Item::BaseInfo* info, bool isRandParam) {

	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((info->TypeID))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(info->TypeID)][idx] == nullptr) {
			// �󂫘g������B

			// �����i�쐬�B
			HoldItemBase* equi = nullptr;
			if (info->TypeID != Item::ItemCodeE::Item) {
				equi = HoldItemFactory::CreateItem(info,isRandParam);
				//�����i��ǉ��B
				_InventoryItemList[static_cast<int>(info->TypeID)][idx] = equi;
				_HoldNumList[static_cast<int>(info->TypeID)][info->ID] += 1;	// �V�����ǉ��������������������v�ɉ��Z�B
				//�����i�̏��������o���B
				_OutData(info->TypeID);
				return static_cast<HoldEquipment*>(equi);;
			}
#ifdef _DEBUG
			else {
				//�A�C�e���͕ʂ̒ǉ��֐����g���āB
				char error[256];
				sprintf(error, "�����̒ǉ���code �A�C�e�����w�肳��܂����B");
				MessageBoxA(0, error, "�����i�ǉ����s", MB_ICONWARNING);
				abort();
			}
#endif
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

bool Inventory::AddEquipment(HoldEquipment* add) {
	for (int idx = 0; idx < _InventoryItemList[static_cast<int>((add->GetInfo()->TypeID))].size(); idx++) {
		if (_InventoryItemList[static_cast<int>(add->GetInfo()->TypeID)][idx] == nullptr) {
			// �󂫘g������B

			//�����i��ǉ��B
			_InventoryItemList[static_cast<int>(add->GetInfo()->TypeID)][idx] = add;

			_HoldNumList[static_cast<int>(add->GetInfo()->TypeID)][add->GetInfo()->ID] += 1;	// �V�����ǉ��������������������v�ɉ��Z�B
			//�����i�̏��������o���B
			_OutData(add->GetInfo()->TypeID);
			return true;
		}
	}

	//�G���[�񍐁B
	// ���b�菈��(�ǉ��ł��Ȃ��ꍇ�͎̂Ă�A�C�e�����v���C���[�ɑI��������K�v������)�B
	{
		char error[256];
		sprintf(error, "�C���x���g������t�Œǉ�����܂���ł����B");
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
	}

	return false;
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
			_InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)][itr - _InventoryItemList[static_cast<int>(item->GetInfo()->TypeID)].begin()] = nullptr;
			return;
		}
	}
}

//�����������炷�B
bool Inventory::SubHoldNum(Item::BaseInfo* item, int num) {
	int work = num;
	bool isDeleteList = false;

	//�z��T�C�Y�������B
	for (auto itr = _InventoryItemList[static_cast<int>(item->TypeID)].begin(); work > 0 && itr != _InventoryItemList[static_cast<int>(item->TypeID)].end();)
	{
		//ID�̈�v�B
		if ((*itr) != nullptr&&item->ID == (*itr)->GetInfo()->ID)
		{
			switch (item->TypeID) {
			case Item::ItemCodeE::Item:
				//�����������i�̐����X�V�B
				work = static_cast<ConsumptionItem*>(*itr)->SubHoldNum(work);
				//�X�V�������ʏ�������0�ɂȂ�Δj���B
				if (static_cast<ConsumptionItem*>(*itr)->GetHoldNum() <= 0) {
					//���X�g����폜�B
					_DeleteFromList(*itr);
					isDeleteList = true;
				}
				break;
			case Item::ItemCodeE::Armor:
			case Item::ItemCodeE::Weapon:
				_DeleteFromList(*itr);
				work--;
				isDeleteList = true;
				break;
			}
			if (isDeleteList) {
				//���X�g����폜���ꂽ�̂ő��̃A�C�e�����l�߂�B
				ArrangementInventory();
			}
		}
		//�s��v�B
		else
		{
			itr++;
		}
	}

	_HoldNumList[static_cast<int>(item->TypeID)][item->ID] -= (num - work);	// �V�����ǉ��������������������v�ɉ��Z�B

	//�����i�̏����������o���B
	_OutData(item->TypeID);

	return isDeleteList;
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
				AddEquipment(static_cast<HoldEquipment*>(HoldItemFactory::CreateItem(work[idx].get())));
			}
			break;
		case static_cast<int>(Item::ItemCodeE::Weapon) :
			// ����ǉ��B
			Support::LoadCSVData<Hold::HoldWeaponInfo, Hold::HoldInfo>(filepath, record[i], recordSize[i], work);
			for (int idx = 0; idx < work.size(); idx++) {
				AddEquipment(static_cast<HoldEquipment*>(HoldItemFactory::CreateItem(work[idx].get())));
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

void Inventory::deleteList() {
	

	for (auto list : _InventoryItemList) {
		list.clear();
	}
	_InventoryItemList.clear();

	_InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(Item::ItemCodeE::Max), vector<HoldItemBase*>(INVENTORYLISTNUM, nullptr));
}