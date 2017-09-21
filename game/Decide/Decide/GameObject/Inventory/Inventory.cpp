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
	Item::BaseInfo* Info = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];


	//�A�C�e���}�l�W���[����ǉ�����A�C�e���̏����擾�B	
	Info = INSTANCE(ItemManager)->GetItemInfo(item->ID, Item::ItemCodeE::Item);

	//�����A�C�e���̃C���X�^���X�쐬�B
	Hold = new ConsumptionItem(Info);

	if (Hold) {
		//�������X�V�B
		static_cast<ConsumptionItem*>(Hold)->UpdateHoldNum(num);
	}


	//�ǉ�����A�C�e���̏�񂪍��ꂽ���`�F�b�N�B
	if (Hold != nullptr) {

		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i] && _InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i]->GetInfo()->ID == Hold->GetInfo()->ID)
			{
				//�����A�C�e�����z��ɂ���B
				static_cast<ConsumptionItem*>(_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i].get())->UpdateHoldNum(num);

				//�����i�̏����������o���B
				_ItemListOutData();
				return;
			}
		}

		// �ǉ�����A�C�e�����z��ɂȂ������B
		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i].get() == nullptr) {
				//�C���x���g���ɋ󂫂�����B

				// �V�����쐬�����A�C�e����z��ɒǉ��B
				_InventoryItemList[static_cast<int>(Item::ItemCodeE::Item)][i].reset(Hold);

				//�����i�z��̏�������CSV�ɏ����o���B
				_ItemListOutData();

				return;
			}
		}


		{
			//�G���[�񍐁B
			char error[256];
			sprintf(error, "�C���x���g������t�Œǉ�����܂���ł����B");
			MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
		}
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
			return _InventoryItemList[static_cast<int>(code)][i].get();
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
		if (item != (*itr).get()) {
			itr++;
		}
		else
		{
			//��v�����̂Œ��g���폜�B
			(*itr).reset(nullptr);
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
			static_cast<ConsumptionItem*>((*itr).get())->UpdateHoldNum(-num);

			//�X�V�������ʏ�������0�ɂȂ�Δj���B
			if (static_cast<ConsumptionItem*>((*itr).get())->GetHoldNum() <= 0) {
				//���X�g����폜�B
				_DeleteFromList((*itr).get());
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

	//CSV�ɏ����o���悤�̃��X�g�B
	vector<vector<unique_ptr<HoldInfo>>> OutPutCSVList = vector<vector<unique_ptr<HoldInfo>>>(static_cast<int>(Item::ItemCodeE::Max));
	FOR(i, ARRAY_SIZE(filename))
	{
		//�t�@�C���p�X�B
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/InventoryData/%s.csv", filename[i]);
		switch (i)
		{
		//�A�C�e���B
		case static_cast<int>((Item::ItemCodeE::Item)):
			for (int idx = 0; idx < _InventoryItemList[static_cast<int>((Item::ItemCodeE::Item))].size(); idx++) {
				if (_InventoryItemList[static_cast<int>((Item::ItemCodeE::Item))][idx]) {
				
					//�����Ă���A�C�e���̏��������o���p�̃��X�g�ɒǉ��B
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
			//ItemList.CSV�ɏ����o���B
			Support::OutputCSV<HoldInfo>(filepath, HoldItemData, ARRAY_SIZE(HoldItemData), OutPutCSVList[static_cast<int>(Item::ItemCodeE::Item)]);
			break;

		//�h��B
		case static_cast<int>(Item::ItemCodeE::Armor):
			for (int idx = 0; idx < _InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)].size(); idx++) {
				if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Armor)][idx]) {

					//�����Ă���h��̏��������o���p�̃��X�g�ɒǉ��B
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

			//ArmorList.CSV�ɏ����o���B
			Support::OutputCSV<HoldInfo>(filepath, HoldArmorData, ARRAY_SIZE(HoldArmorData), OutPutCSVList[static_cast<int>(Item::ItemCodeE::Armor)]);
			break;

		//����B
		case static_cast<int>(Item::ItemCodeE::Weapon):

			for (int idx = 0; idx < _InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)].size(); idx++) {
				if (_InventoryItemList[static_cast<int>(Item::ItemCodeE::Weapon)][idx]) {

					//�����Ă��镐��̏��������o���p�̃��X�g�ɒǉ��B
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
			//WeaponList.CSV�ɏ����o���B
			Support::OutputCSV<HoldInfo>(filepath, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), OutPutCSVList[static_cast<int>(Item::ItemCodeE::Weapon)]);
			break;
		}
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
			_InventoryItemList[static_cast<int>(code)][idx].reset(equi);

			//�����i�̏��������o���B
			_ItemListOutData();
			break;
		}
	}	
}