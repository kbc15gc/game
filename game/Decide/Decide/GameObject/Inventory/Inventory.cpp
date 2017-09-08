#include "stdafx.h"
#include "GameObject\ItemManager\ItemManager.h"
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
	for (int i = 0; i < 2; i++)
	{
		AddItem(ItemManager::ItemKodeE::Item, INSTANCE(ItemManager)->GetItemInfo(i, ItemManager::ItemKodeE::Item));
	}
	//_NowLookItemPos = 0;
	//UseItem();
}

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::AddItem(ItemManager::ItemKodeE kode, Item::BaseInfo* item) {
	Item::BaseInfo* Item = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];

	//�A�C�e���R�[�h�����č��A�C�e�������߂�B
	switch (kode)
	{
		//�����A�C�e���B
	case ItemManager::ItemKodeE::Item:

		//�A�C�e���}�l�W���[����ǉ�����A�C�e���̏����擾�B	
		Item = (Item::ItemInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, kode);
		//�����A�C�e���̃C���X�^���X�쐬�B
		Hold = (ConsumptionItem*)INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("ConsumptionItem", 5);
		//�����A�C�e���ɒǉ�����A�C�e���̏����i�[�B
		Hold->_Info = Item;
		break;

		//�����h��B
	case ItemManager::ItemKodeE::Armor:

		//�A�C�e���}�l�W���[����ǉ�����h��̏����擾�B	
		Item = (Item::ArmorInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, kode);
		//�����h��̃C���X�^���X�쐬�B
		Hold = (HoldArmor*)INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 5);
		//�����h��ɒǉ�����h��̏����i�[�B
		Hold->_Info = Item;
		break;

		//��������B
	case ItemManager::ItemKodeE::Weapon:

		//�A�C�e���}�l�W���[����ǉ����镐��̏����擾�B	
		Item = (Item::WeaponInfo*)INSTANCE(ItemManager)->GetItemInfo(item->ID, kode);;
		//��������̃C���X�^���X�쐬�B
		Hold = (HoldWeapon*)INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 5);
		//��������ɒǉ����镐��̏����i�[�B
		Hold->_Info = Item;
		break;

	case ItemManager::ItemKodeE::Max:

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
			if (_InventoryItemList[(int)kode][j] == NULL) {

				//�ǉ��B
				_InventoryItemList[(int)kode][j] = Hold;
				_InventoryItemList[(int)kode][j]->_Info->HoldNum++;
				return;
			}
			else
			{
				//�ǉ�����ۂɓ����A�C�e���������ē����Ȃ珊���������B
				if (_InventoryItemList[(int)kode][j]->_Info->ID == Hold->_Info->ID)
				{
					//�������X�V�B
					_InventoryItemList[(int)kode][j]->_Info->HoldNum++;
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
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)ItemManager::ItemKodeE::Item][_NowLookItemPos];
}

HoldItemBase* Inventory::FindItem(ItemManager::ItemKodeE kode, const unsigned int& id) {
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		if (_InventoryItemList[(int)kode][i]->_Info->ID == id) {
			return _InventoryItemList[(int)kode][i];
		}
	}
	return nullptr;
}