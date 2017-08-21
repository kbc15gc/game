#pragma once

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

	//�����o�ϐ��̏��ݒ�
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

	//�����o�ϐ��̏��ݒ�
	static Support::DATARECORD  Weapondata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct WeaponInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ATK),		sizeof(int) },
	};
}

//�A�C�e���̏����Ǘ�����N���X
class ItemManager
{
public:
	ItemManager();

	void LoadAllData();

	//�A�C�e���̏���ǂݍ��݁B
	void LoadItemData();

	//�h��̏���ǂݍ��݁B
	void LoadArmorData();

	//����̏���ǂݍ��݁B
	void LoadWeaponData();

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
			_Instance->LoadItemData();
		}
		return _Instance;
	}
private:
	//�A�C�e���̃��X�g
	vector<unique_ptr<Item::ItemInfo>> _ItemList[3];

	//�A�C�e���̃��X�g
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//�h��̃��X�g
	vector<unique_ptr<Armor::ArmorInfo>> _ArmorList;

	//����̃��X�g
	vector<unique_ptr<Weapon::WeaponInfo>> _WeaponList;
	
	static ItemManager* _Instance;
};