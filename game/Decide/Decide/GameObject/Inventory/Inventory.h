#pragma once
#include "GameObject\ItemManager\ItemManager.h"
class SoundSource;
class HoldItemBase;

const int INVENTORYLISTNUM = 5;

//�C���x���g���N���X�B
class Inventory
{
private:
	Inventory();

public:

	void Initialize();

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->Initialize();
		}
		return _InventoryInstance;
	}


	//�v���C���[�̏��������擾�B
	int GetPlayerMoney() {
		return _PlayerMoney;
	}

	int* GetPlayerMoneyPt()
	{
		return &_PlayerMoney;
	}

	//�v���C���[�̏������ɉ��Z�B
	void AddPlayerMoney(int add)
	{
		_PlayerMoney += add;
	}

	//�v���C���[�̏��������猸�Z�B
	void SubtractPlayerMoney(int sub) {
		_PlayerMoney -= sub;
	}

	//�A�C�e�����C���x���g���ɒǉ��B
	void AddItem(ItemManager::ItemCodeE code, Item::BaseInfo* item);

	//�w�肳�ꂽ�C���x���g���̃��X�g�̐擪���擾�B
	inline const vector<HoldItemBase*>& GetInventoryList(ItemManager::ItemCodeE code) {
		return _InventoryItemList[(int)code];
	}

	 //�A�C�e���R�[�h��ID�����ɔz�񂩂猟���B
	 HoldItemBase* FindItem(ItemManager::ItemCodeE code, const unsigned int& id);

	
	//���X�g����w�肳�ꂽ�A�C�e�����폜�B
	void DeleteFromList(ItemManager::ItemCodeE code, const unsigned int& id);

	inline vector<Item::BaseInfo*> GetInfoList() {
		return _InfoList;
	}
	
	void UseItem();
private:
	
	//�C���x���g���B
	vector<vector<HoldItemBase*>> _InventoryItemList = vector<vector<HoldItemBase*>>(static_cast<int>(ItemManager::ItemCodeE::Max), vector<HoldItemBase*>(INVENTORYLISTNUM, nullptr));

	//�C���x���g���̃A�C�e���̏��B
	vector<Item::BaseInfo*> _InfoList;

	//�����Ă���A�C�e���B
	int _NowLookItemPos = -1;

	//�������B
	int _PlayerMoney = 100;

	static Inventory* _InventoryInstance;
};