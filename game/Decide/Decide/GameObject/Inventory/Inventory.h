#pragma once

namespace PlayerInventory
{
	struct BaseInfo :public Noncopyable
	{
		int TypeID;				//��ށB
		int ID;					//�A�C�e��ID�B
		char Name[256];			//�A�C�e�����B
		char Description[256];	//�A�C�e���̐��B
		int Value;				//�l�i�B
		int HoldNum;			//�������B
	};
	//�C���x���g�����ɂ���A�C�e���B
	//�A�C�e���̏����܂Ƃ߂��\���́B
	struct ItemInfo :BaseInfo
	{
		
		int Recovery;			//�򑐂��g�������̉񕜗ʁB
		int AtkBuff;			//�򑐂��g�������̍U���͂̏㏸�ʁB
		int DefBuff;			//�򑐂��g�������̖h��͂̏㏸�ʁB
		int SpeedBuff;			//�򑐂��g�������̈ړ����x�̏㏸�ʁB
		
	};

	//�C���x���g�����ɂ���h��B
	//�h��̏����܂Ƃ߂��\���́B
	struct ArmorInfo :BaseInfo
	{
		//int TypeID;				//��ށB
		//int ID;					//�h��ID�B
		//char Name[256];			//�h��B
		//char Description[256];	//�h��̐��B
		//int Value;				//�l�i�B
		int ATK;				//�h��𑕔��������ɏオ��U���́B
		int DEF;				//�h��𑕔��������ɏオ��h��́B
		//int HoldNum;			//�������B
	};

	//�C���x���g�����ɂ��镐��B�B
	//�v���C���[�������Ă��镐��̏����܂Ƃ߂��\���́B
	struct WeaponInfo :BaseInfo
	{
		//int TypeID;				//��ށB
		//int ID;					//����ID�B
		//char Name[256];			//���햼�B
		//char Description[256];	//����̐��B
		//int Value;				//�l�i�B
		int ATK;				//����𑕔��������ɏオ��U���́B
		//int HoldNum;			//�������B
	};
}
//�C���x���g���̘g���B
const int INVENTORYLISTNUM = 5;

#include"GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include <tuple>


//�C���x���g���N���X�B
class Inventory
{
private:
	Inventory();

	//�v���C���[�̃A�C�e���̃C���x���g�����������B
	void _PlayerItemListInitialize(int i);

	//�v���C���[�̖h��̃C���x���g�����������B
	void _PlayerArmorListInitialize(int i);

	//�v���C���[�̕���̃C���x���g�����������B
	void _PlayerWeaponListInitialize(int i);

	//�A�C�e�����C���x���g���̋󂢂Ă���ꏊ�ɒǉ��B
	void _AddItem(int AddPos, Item::ItemInfo *item = nullptr);

	//�h����C���x���g���ɒǉ��B
	void _AddArmor(int AddPos, Item::ArmorInfo *armor = nullptr);

	//������C���x���g���ɒǉ��B
	void _AddWeapon(int AddPos, Item::WeaponInfo *weapon = nullptr);

	//�ǉ�����A�C�e�����A�C�e���̃C���x���g������T������Ώ����������𑝂₵�A�Ȃ���΋󂢂Ă���ꏊ�ɒǉ����A�ǂ���ł��Ȃ���΃G���[���o���B
	void _ItemAddCheckAndPos(Item::ItemInfo *item = nullptr);

	//�ǉ�����h���h��̃C���x���g������T������Ώ����������𑝂₵�A�Ȃ���΋󂢂Ă���ꏊ�ɒǉ����A�ǂ���ł��Ȃ���΃G���[���o���B
	void _ArmorAddCheckAndPos(Item::ArmorInfo *item = nullptr);

	//�ǉ����镐��𕐊�̃C���x���g������T������Ώ����������𑝂₵�A�Ȃ���΋󂢂Ă���ꏊ�ɒǉ����A�ǂ���ł��Ȃ���΃G���[���o���B
	void _WeaponAddCheckAndPos(Item::WeaponInfo *item = nullptr);

public:

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->Initalize();
		}
		return _InventoryInstance;
	}

	//�e�C���x���g���̏������B
	void Initalize();

	//�C���x���g���ɃA�C�e����ǉ�����B
	void AddInventory(Item::BaseInfo *item = nullptr);

	//�v���C���[�̃A�C�e���̃C���x���g�����擾�B
	PlayerInventory::ItemInfo* GetPlayerItemList() {
		return &_PlayerItemList[0];
	}

	//�v���C���[�̖h��̃C���x���g�����擾�B
	PlayerInventory::ArmorInfo* GetPlayerAromorList()
	{
		return &_PlayerArmorList[0];
	}

	//�v���C���[�̕���̃C���x���g�����擾�B
	PlayerInventory::WeaponInfo* GetPlayerWeaponList()
	{

		return &_PlayerWeaponList[0];
	}

	//�~�����C���x���g�����w�肷��Ƃ��̃C���x���g�����擾�B
	PlayerInventory::BaseInfo* GetInventory(ItemManager::ItemKodeE kode)
	{
		switch (kode)
		{
			//�A�C�e���̃C���x���g�����擾�B
		case ItemManager::ItemKodeE::Item:
			return &_PlayerItemList[0];
			break;

			//�h��̃C���x���g�����擾�B
		case ItemManager::ItemKodeE::Armor:
			return &_PlayerArmorList[0];
			break;

			//����̃C���x���g�����擾�B
		case ItemManager::ItemKodeE::Weapon:
			return &_PlayerWeaponList[0];
			break;

			//��O�����B
		default:
			char error[256];
			sprintf(error, "�w�肳�ꂽ�A�C�e���R�[�h�������ł��B");
			MessageBoxA(0, error, "�w�肳�ꂽ�A�C�e���̃C���x���g���擾���s", MB_ICONWARNING);
			break;
		}
	}

	//�w�肳�ꂽ�ꏊ�̃A�C�e�����̂Ă�B
	void ItemThrowAway(int pos) {
		_PlayerItemListInitialize(pos);

		//���Đ��B
		_AddOrOutSE->Play(false);
	}

	//�w�肳�ꂽ�ꏊ�̖h����̂Ă�B
	void ArmorThrowAway(int pos) {
		_PlayerArmorListInitialize(pos);

		//���Đ��B
		_AddOrOutSE->Play(false);
	}

	//�w�肳�ꂽ�ꏊ�̕�����̂Ă�B
	void WeaponThrowAway(int pos) {
		_PlayerWeaponListInitialize(pos);

		//���Đ��B
		_AddOrOutSE->Play(false);
	}

	//�A�C�e�����g���B
	//�߂�l�͍�����񕜗ʁA�U���̓o�t�A�h��̓o�t�A�ړ����x�o�t�B
	//�߂�l�̎󂯎���:int recover,int atk,int def,int speed;tie(recover,atk,def,speed)=UseItem();
	tuple<int, int, int, int> UseItem(int pos);

	//�I�����ꂽ�A�C�e���̃C���x���g�������炷�B
	void ItemListSelectPosUp() {
		_ItemListSelectPos++;
	}

	//�I�����ꂽ�A�C�e���̃C���x���g�������炷�B
	void ItemListSelectPosDown() {
		_ItemListSelectPos--;
	}

	//�I�����ꂽ�h��̃C���x���g�������炷�B
	void ArmorListSelectPosUp() {
		_ArmorListSelectPos++;
	}

	//�I�����ꂽ�h��̃C���x���g�������炷�B
	void ArmorListSelectPosDown() {
		_ArmorListSelectPos--;
	}

	//�I�����ꂽ����̃C���x���g�������炷�B
	void WeaponListSelectPosUp() {
		_WeaponListSelectPos++;
	}

	//�I�����ꂽ����̃C���x���g�������炷�B
	void WeaponListSelectPosDown() {
		_WeaponListSelectPos--;
	}

	//�v���C���[�̏��������擾�B
	int GetPlayerMoney() {
		return _PlayerMoney;
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

private:
	//�v���C���[�̃A�C�e���̃C���x���g���B
	PlayerInventory::ItemInfo	_PlayerItemList[INVENTORYLISTNUM];

	//�v���C���[�̖h��̃C���x���g���B
	PlayerInventory::ArmorInfo	_PlayerArmorList[INVENTORYLISTNUM];

	//�v���C���[�̕���̃C���x���g���B
	PlayerInventory::WeaponInfo	_PlayerWeaponList[INVENTORYLISTNUM];

	//���C���x���g���̂ǂ���I�����Ă��邩�B
	int _ItemListSelectPos, _ArmorListSelectPos, _WeaponListSelectPos = 0;

	//�������B
	int _PlayerMoney = 100;

	//�C���x���g���ɒǉ�or���o�����̉��B
	SoundSource* _AddOrOutSE = nullptr;

	static Inventory* _InventoryInstance;
};