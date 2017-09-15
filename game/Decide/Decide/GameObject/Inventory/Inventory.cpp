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


	////�t�@�C���p�X
	//char filepath[256] = "";
	//strcpy(filepath, "Asset/Data/InventoryData/ItemList.csv");
	////�t�@�C������A�C�e�����ǂݍ��݁B
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

	////�t�@�C���p�X
	//char filepath2[256] = "";
	//strcpy(filepath2, "Asset/Data/InventoryData/ArmorList.csv");
	////�t�@�C������A�C�e�����ǂݍ��݁B
	//vector<unique_ptr<HoldArmorInfo>> work2;
	//Support::LoadCSVData<HoldArmorInfo>(filepath2, HoldArmorData, ARRAY_SIZE(HoldArmorData), work2);


	////�t�@�C���p�X
	//char filepath3[256] = "";
	//strcpy(filepath3, "Asset/Data/InventoryData/WeaponList.csv");
	////�t�@�C������A�C�e�����ǂݍ��݁B
	//vector<unique_ptr<HoldWeponInfo>> work3;
	//Support::LoadCSVData<HoldWeponInfo>(filepath3, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), work3);


}

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::AddItem(Item::ItemCodeE code, Item::BaseInfo* item,int num) {
	Item::BaseInfo* Info = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];

	//�A�C�e���R�[�h�����č��A�C�e�������߂�B
	{
		//�A�C�e���}�l�W���[����ǉ�����A�C�e���̏����擾�B	
		Info = INSTANCE(ItemManager)->GetItemInfo(item->ID, code);
		switch (code)
		{
		case Item::ItemCodeE::Item:
			//�����A�C�e���B

			//�����A�C�e���̃C���X�^���X�쐬�B
			Hold = new ConsumptionItem(Info);
			break;

		case Item::ItemCodeE::Armor:
			//�����h��B

			//�����h��̃C���X�^���X�쐬�B
			Hold = new HoldArmor(Info);
			break;

		case Item::ItemCodeE::Weapon:
			//��������B

			//��������̃C���X�^���X�쐬�B
			Hold = new HoldWeapon(Info);
			break;
		default:
			//�G���[�񍐁B
			sprintf(error, "�����ȃA�C�e���R�[�h���n����܂����B");
			MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
			break;
		}

		if (Hold) {
			//�������X�V�B
			Hold->UpdateHoldNum(num);
		}
	}

	//�ǉ�����A�C�e���̏�񂪍��ꂽ���`�F�b�N�B
	if (Hold != nullptr) {

		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[(int)code][i] && _InventoryItemList[(int)code][i]->GetInfo()->ID == Hold->GetInfo()->ID)
			{
				//�����A�C�e�����z��ɂ���B
				_InventoryItemList[(int)code][i]->UpdateHoldNum(num);

				//�����i�̏����������o���B
				_ItemListOutData();
				return;
			}
		}

		// �ǉ�����A�C�e�����z��ɂȂ������B
		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			if (_InventoryItemList[(int)code][i].get() == nullptr) {
				//�C���x���g���ɋ󂫂�����B

				// �V�����쐬�����A�C�e����z��ɒǉ��B
				_InventoryItemList[(int)code][i].reset(Hold);

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
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)Item::ItemCodeE::Item][_NowLookItemPos];
}

//�A�C�e���R�[�h��ID�����ɔz�񂩂猟���B
HoldItemBase* Inventory::FindItem(Item::ItemCodeE kode, const unsigned int& id) {

	//�z��T�C�Y�������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//�����B
		if (_InventoryItemList[(int)kode][i] && _InventoryItemList[(int)kode][i]->GetInfo()->ID == id) {
			return _InventoryItemList[(int)kode][i].get();
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
	for (auto itr = _InventoryItemList[(int)item->GetInfo()->TypeID].begin() ; itr != _InventoryItemList[(int)item->GetInfo()->TypeID].end();)
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
	for (auto itr = _InventoryItemList[(int)item->TypeID].begin(); itr != _InventoryItemList[(int)item->TypeID].end();)
	{
		//ID�̈�v�B
		if ((*itr) != nullptr&&item->ID == (*itr)->GetInfo()->ID)
		{
			//�����������i�̐����X�V�B
			(*itr)->UpdateHoldNum(-num);

			//�X�V�������ʏ�������0�ɂȂ�Δj���B
			if ((*itr)->GetHoldNum() <= 0) {
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

	////�t�@�C���l�[��
	//const char* filename[] = { "ItemList","ArmorList","WeaponList" };
	//vector<unique_ptr<HoldInfo>> list;
	//FOR(i, ARRAY_SIZE(filename))
	//{
	//	//�t�@�C���p�X
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
	//		//�t�@�C���p�X
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
	//		//�t�@�C���p�X
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
	//		//�t�@�C���p�X
	//		char filepath3[256] = "";
	//		strcpy(filepath3, "Asset/Data/InventoryData/WeaponList.csv");
	//		Support::OutputCSV<HoldWeponInfo>(filepath3, HoldWeaponData, ARRAY_SIZE(HoldWeaponData), work3);
	//	}
	//}
}