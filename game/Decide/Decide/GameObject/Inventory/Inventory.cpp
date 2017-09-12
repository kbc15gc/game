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
	AddItem(Item::ItemCodeE::Item, INSTANCE(ItemManager)->GetItemInfo(0, Item::ItemCodeE::Item));
	SubHoldNum(FindItem(Item::ItemCodeE::Item,0)->GetInfo(), -1);
}

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::AddItem(Item::ItemCodeE code, Item::BaseInfo* item) {
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
		case Item::ItemCodeE::Item:
			//�����A�C�e���̃C���X�^���X�쐬�B
			Hold = (ConsumptionItem*)INSTANCE(GameObjectManager)->AddNew<ConsumptionItem>("ConsumptionItem", 5);
			break;

			//�����h��B
		case Item::ItemCodeE::Armor:
			//�����h��̃C���X�^���X�쐬�B
			Hold = (HoldArmor*)INSTANCE(GameObjectManager)->AddNew<HoldArmor>("HoldArmor", 5);
			break;

			//��������B
		case Item::ItemCodeE::Weapon:
			//��������̃C���X�^���X�쐬�B
			Hold = (HoldWeapon*)INSTANCE(GameObjectManager)->AddNew<HoldWeapon>("HoldWeapon", 5);
			break;
		default:
			//�G���[�񍐁B
			sprintf(error, "�����ȃA�C�e���R�[�h���n����܂����B");
			MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
			break;
		}
		//�����i�ɒǉ�����A�C�e���̏����i�[�B
		Hold->SetInfo(Item);
	}

	//�ǉ�����A�C�e���̏�񂪍��ꂽ���`�F�b�N�B
	if (Hold != nullptr) {

		for (int i = 0; i < INVENTORYLISTNUM; i++)
		{
			//�C���x���g���ɉ��������Ȃ�ǉ��B
			if (_InventoryItemList[(int)code][i] == NULL) {

				//�ǉ��B
				_InventoryItemList[(int)code][i] = Hold;

				//�������X�V�B
				_InventoryItemList[(int)code][i]->AddHoldNum();

				//�ǉ����ꂽ�A�C�e���̏����i�[�B
				_InfoList.push_back(Item);
				return;
			}
			else
			{
				//�ǉ�����ۂɓ����A�C�e���������ē����Ȃ珊���������B
				if (_InventoryItemList[(int)code][i]->GetInfo()->ID == Hold->GetInfo()->ID)
				{
					//�������X�V�B
					_InventoryItemList[(int)code][i]->AddHoldNum();
					return;
				}
			}

			if (i == INVENTORYLISTNUM)
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
	//HoldItemBase* item = (ConsumptionItem*)_InventoryItemList[(int)Item::ItemCodeE::Item][_NowLookItemPos];
}

//�A�C�e���R�[�h��ID�����ɔz�񂩂猟���B
HoldItemBase* Inventory::FindItem(Item::ItemCodeE kode, const unsigned int& id) {

	//�z��T�C�Y�������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//�����B
		if (_InventoryItemList[(int)kode][i]->GetInfo()->ID == id) {
			return _InventoryItemList[(int)kode][i];
		}
	}

	//������Ȃ������B
	char error[256];
	sprintf(error, "�w�肳�ꂽ�A�C�e����������܂���ł����B");
	MessageBoxA(0, error, "�A�C�e���̌������s", MB_ICONWARNING);
}

//���X�g����w�肳�ꂽ�A�C�e�����폜�B
void Inventory::DeleteFromList(HoldItemBase* item) {

	//�z��T�C�Y�������B
	for (auto itr = _InventoryItemList[(int)item->GetInfo()->TypeID].begin() ; itr != _InventoryItemList[(int)item->GetInfo()->TypeID].end();)
	{
		if (item != *itr) {
			itr++;
		}
		else
		{
			INSTANCE(GameObjectManager)->AddRemoveList(FindItem(item->GetInfo()->TypeID, item->GetInfo()->ID));
			*itr = nullptr;
			//itr = _InventoryItemList[(int)item->GetInfo()->TypeID].erase(itr);
		}
	}

	//��񂾂����i�[�������X�g�̃A�C�e�����폜�B
	for (auto itr = _InfoList.begin(); itr != _InfoList.end();)
	{
		//�A�C�e���R�[�h��ID����v�B
		if (item->GetInfo()->TypeID == (*itr)->TypeID&&
			item->GetInfo()->ID == (*itr)->ID) {

			//�폜�B
			itr = _InfoList.erase(itr);
			return;
		}
		//�s��v�B
		else
		{
			itr++;
		}
	}
}

//�����������炷�B
void Inventory::SubHoldNum(Item::BaseInfo* item, int num) {
	//�z��T�C�Y�������B
	for (auto itr = _InventoryItemList[(int)item->TypeID].begin(); itr != _InventoryItemList[(int)item->TypeID].end();)
	{
		//ID�̈�v�B
		if (item->ID == (*itr)->GetInfo()->ID) 
		{
			//�����������i�̐����X�V�B
			(*itr)->AddHoldNum(num);

			//�X�V�������ʏ�������0�ɂȂ�Δj���B
			if ((*itr)->GetHoldNum() <= 0);
			DeleteFromList(*itr);
			return;
		}
		//�s��v�B
		else
		{
			itr++;
		}
	}
}