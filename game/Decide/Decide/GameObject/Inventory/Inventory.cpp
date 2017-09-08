#include "stdafx.h"
#include "Inventory.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "ConsumptionItem.h"
#include "HoldArmor.h"
#include "HoldWeapon.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

void Inventory::Initialize() {
	
}

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::AddItem(ItemManager::ItemCodeE code, Item::BaseInfo* item) {
	Item::BaseInfo* Item = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];

	//�A�C�e���R�[�h�����č��A�C�e�������߂�B
	switch (code)
	{
		//�����A�C�e���B
	case ItemManager::ItemCodeE::Item:

		//�A�C�e���}�l�W���[����ǉ�����A�C�e���̏����擾�B	
		Item = (Item::ItemInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, code);
		//�����A�C�e���̃C���X�^���X�쐬�B
		Hold = (ConsumptionItem*)INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("ConsumptionItem", 5);
		//�����A�C�e���ɒǉ�����A�C�e���̏����i�[�B
		Hold->_Info = Item;
		break;

		//�����h��B
	case ItemManager::ItemCodeE::Armor:

		//�A�C�e���}�l�W���[����ǉ�����h��̏����擾�B	
		Item = (Item::ArmorInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, code);
		//�����h��̃C���X�^���X�쐬�B
		Hold = (HoldArmor*)INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 5);
		//�����h��ɒǉ�����h��̏����i�[�B
		Hold->_Info = Item;
		break;

		//��������B
	case ItemManager::ItemCodeE::Weapon:

		//�A�C�e���}�l�W���[����ǉ����镐��̏����擾�B	
		Item = (Item::WeaponInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, code);;
		//��������̃C���X�^���X�쐬�B
		Hold = (HoldWeapon*)INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 5);
		//��������ɒǉ����镐��̏����i�[�B
		Hold->_Info = Item;
		break;

	case ItemManager::ItemCodeE::Max:

		//�G���[�񍐁B
		sprintf(error, "�����ȃA�C�e���R�[�h���n����܂����B");
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
		break;
	default:
		//�G���[�񍐁B
		sprintf(error, "�����ȃA�C�e���R�[�h���n����܂����B");
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
		break;
	}

	//�ǉ�����A�C�e���̏�񂪍��ꂽ���`�F�b�N�B
	if (Hold != nullptr) {

		for (int j = 0; j < INVENTORYLISTNUM; j++)
		{
			//�C���x���g���ɉ��������Ȃ�ǉ��B
			if (_InventoryItemList[(int)code][j] == NULL) {

				//�ǉ��B
				_InventoryItemList[(int)code][j] = Hold;
				_InventoryItemList[(int)code][j]->_Info->HoldNum++;
				return;
			}
			else
			{
				//�ǉ�����ۂɓ����A�C�e���������ē����Ȃ珊���������B
				if (_InventoryItemList[(int)code][j]->_Info->ID == Hold->_Info->ID)
				{
					//�������X�V�B
					_InventoryItemList[(int)code][j]->_Info->HoldNum++;
					return;
				}
			}

			if (j == INVENTORYLISTNUM)
			{
				//�G���[�񍐁B
				char error[256];
				sprintf(error, "�C���x���g������t�Œǉ�����܂���ł����B");
				MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
			}
		}
	}
}

void Inventory::UseItem() {
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)ItemManager::ItemCodeE::Item][_NowLookItemPos];
}

//�A�C�e���R�[�h��ID�����ɔz�񂩂猟���B
HoldItemBase* Inventory::FindItem(ItemManager::ItemCodeE kode, const unsigned int& id) {

	//�z��T�C�Y�������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//�����B
		if (_InventoryItemList[(int)kode][i]->_Info->ID == id) {
			return _InventoryItemList[(int)kode][i];
		}
	}

	//������Ȃ������B
	char error[256];
	sprintf(error, "�w�肳�ꂽ�A�C�e����������܂���ł����B");
	MessageBoxA(0, error, "�A�C�e���̌������s", MB_ICONWARNING);
	return nullptr;
}