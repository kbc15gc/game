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
}

void Inventory::Initialize() {
	AddItem(ItemManager::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(0, ItemManager::ItemCodeE::Item));
	DeleteFromList(ItemManager::ItemCodeE::Item, 0);
}

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::AddItem(ItemManager::ItemCodeE code, Item::BaseInfo* item) {
	Item::BaseInfo* Item = nullptr;
	HoldItemBase* Hold = nullptr;
	char error[256];

	//�A�C�e���R�[�h�����č��A�C�e�������߂�B
	{
		//�A�C�e���}�l�W���[����ǉ�����A�C�e���̏����擾�B	
		Item = INSTANCE(ItemManager)->GetItemInfo(item->ID, code);
		switch (code)
		{
			//�����A�C�e���B
		case ItemManager::ItemCodeE::Item:
			//�����A�C�e���̃C���X�^���X�쐬�B
			Hold = (ConsumptionItem*)INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("ConsumptionItem", 5);
			break;

			//�����h��B
		case ItemManager::ItemCodeE::Armor:
			//�����h��̃C���X�^���X�쐬�B
			Hold = (HoldArmor*)INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 5);
			break;

			//��������B
		case ItemManager::ItemCodeE::Weapon:
			//��������̃C���X�^���X�쐬�B
			Hold = (HoldWeapon*)INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 5);
			break;
		default:
			//�G���[�񍐁B
			sprintf(error, "�����ȃA�C�e���R�[�h���n����܂����B");
			MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
			break;
		}
		//��������ɒǉ����镐��̏����i�[�B
		Hold->_Info = Item;

		//�ǉ����ꂽ�A�C�e���̏����i�[�B
		_InfoList.push_back(Item);
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
}

void Inventory::DeleteFromList(ItemManager::ItemCodeE code, const unsigned int& id) {

	//�z��T�C�Y�������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		SAFE_DELETE(_InventoryItemList[(int)code][i]);
	}
}