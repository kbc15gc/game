#pragma once
#include<tuple>
namespace Item
{
	//�A�C�e���̏����܂Ƃ߂��\���́B
	struct ItemInfo :Noncopyable
	{
		int ID;					//�A�C�e��ID�B
		char Name[256];			//�A�C�e�����B
		char Description[256];	//�A�C�e���̐��B
		int Value;				//�l�i�B
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD Itemdata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ItemInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ItemInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,Value),		sizeof(int) },
	};
}

namespace Armor
{
	//�h��̏����܂Ƃ߂��\���́B
	struct ArmorInfo :Noncopyable
	{
		int ID;					//�h��ID�B
		char Name[256];			//�h��B
		char Description[256];	//�h��̐��B
		int Value;				//�l�i�B
		int ATK;				//�h��𑕔��������ɏオ��U���́B
		int DEF;				//�h��𑕔��������ɏオ��h��́B
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD Armordata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ArmorInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ArmorInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,ATK),		sizeof(int) },
		{ "DEF",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,DEF),		sizeof(int) },
	};
}


namespace Weapon
{
	//����̏����܂Ƃ߂��\���́B
	struct WeaponInfo :Noncopyable
	{
		int ID;					//����ID�B
		char Name[256];			//���햼�B
		char Description[256];	//����̐��B
		int Value;				//�l�i�B
		int ATK;				//����𑕔��������ɏオ��U���́B
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD  Weapondata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct WeaponInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ATK),		sizeof(int) },
	};
}

//�A�C�e���̏����Ǘ�����N���X�B
class ItemManager
{
public:
	ItemManager();

	enum class ItemKodeE
	{
		Invalid = -1,	//�����B
		Item = 0,		//�A�C�e���B
		Armor = 1,		//�h��B
		Weapon = 2,		//����B
	};

	//�A�C�e���A����A�h����ꊇ�œǂݍ��݁B
	void LoadAllItemData();

	Item::ItemInfo* GetItem(const unsigned int& id,const unsigned int& type)
	{
		//�͈̓`�F�b�N
		if (type < 2)
			return _ItemList[type].at(id).get();
		else
			return nullptr;
	}
	
	static ItemManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new ItemManager();
			//�A�C�e���̏���ǂݍ��݁B
			_Instance->LoadAllItemData();
		}
		return _Instance;
	}


	//�v���C���[�̏����i�ɃA�C�e����ǉ��B��1����:�ǉ�����A�C�e���̎�ށA��2����:�A�C�e��ID�A��3����:���O�A��4����:�A�C�e���̐����A��5����:�l�i�A�A��6����:�U���́A��7����:�h��́B
	void AddPlayerItemList(ItemKodeE kode, int* id = nullptr, const char* name = nullptr, const char* des = nullptr, int* value = nullptr, int* atk = nullptr, int* def = nullptr);

	//�v���C���[���������Ă���h��̃��X�g���擾�B
	//Armor::ArmorInfo *GetPlayerAromorInfo();


private:
	//�A�C�e���̃��X�g�B
	vector<unique_ptr<Item::ItemInfo>> _ItemList[3];

	//�A�C�e���̃��X�g�B
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//�h��̃��X�g�B
	vector<unique_ptr<Armor::ArmorInfo>> _ArmorList;

	//����̃��X�g�B
	vector<unique_ptr<Weapon::WeaponInfo>> _WeaponList;

	//�v���C���[���������Ă���A�C�e���̃��X�g�B
	Item::ItemInfo _PlayerItemList[5];

	//�v���C���[���������Ă���h��̃��X�g�B
	Armor::ArmorInfo _PlayerArmorList[5];

	Armor::ArmorInfo arm[2];

	//�v���C���[���������Ă��镐��̃��X�g�B
	Weapon::WeaponInfo _PlayerWeaponList[5];

	//�����i���i�[���郊�X�g�̍ő吔�B
	int ListNum = 5;

	//�v���C���[�������Ă���A�C�e���A�h��A����̐��B
	int ItemCounter, ArmorCounter, WeapoCounter = 0;
	
	static ItemManager* _Instance;
};