#pragma once

namespace Item {

	struct BaseInfo :public Noncopyable
	{
		int TypeID;				//���(�A�C�e����0)�B
		int ID;					//�A�C�e��ID�B
		char Name[256];			//�A�C�e�����B
		char Description[256];	//�A�C�e���̐��B
		int Value;				//�l�i�B
	};
	//�A�C�e���̏����܂Ƃ߂��\���́B
	struct ItemInfo :public BaseInfo
	{
		
		int Recovery;			//�򑐂��g�������̉񕜗ʁB
		int AtkBuff;			//�򑐂��g�������̍U���͂̏㏸�ʁB
		int DefBuff;			//�򑐂��g�������̖h��͂̏㏸�ʁB
		int SpeedBuff;			//�򑐂��g�������̈ړ����x�̏㏸�ʁB
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD Itemdata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct ItemInfo,TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ItemInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ItemInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,Value),		sizeof(int) },
		{ "Recovery",Support::DataTypeE::INT ,		offsetof(struct ItemInfo,Recovery),		sizeof(int) },
		{ "AtkBuff",Support::DataTypeE::INT ,		offsetof(struct ItemInfo,AtkBuff),		sizeof(int) },
		{ "DefBuff",Support::DataTypeE::INT ,		offsetof(struct ItemInfo,DefBuff),		sizeof(int) },
		{ "SpeedBuff",Support::DataTypeE::INT ,		offsetof(struct ItemInfo,SpeedBuff),		sizeof(int) },
	};

	//�h��̏����܂Ƃ߂��\���́B
	struct ArmorInfo :public BaseInfo
	{
		//int TypeID;				//���(�h���1)�B
		//int ID;					//�h��ID�B
		//char Name[256];			//�h��B
		//char Description[256];	//�h��̐��B
		//int Value;				//�l�i�B
		int ATK;				//�h��𑕔��������ɏオ��U���́B
		int DEF;				//�h��𑕔��������ɏオ��h��́B
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD Armordata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct ArmorInfo,TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ArmorInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ArmorInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,ATK),		sizeof(int) },
		{ "DEF",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,DEF),		sizeof(int) },
	};


	//����̏����܂Ƃ߂��\���́B
	struct WeaponInfo :public BaseInfo
	{
		//int TypeID;				//���(�����2)�B
		//int ID;					//����ID�B
		//char Name[256];			//���햼�B
		//char Description[256];	//����̐��B
		//int Value;				//�l�i�B
		int ATK;				//����𑕔��������ɏオ��U���́B
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD  Weapondata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct WeaponInfo,TypeID),			sizeof(int) },
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
private:
	ItemManager();
public:
	//ItemManager();

	enum class ItemKodeE
	{
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

	//�w�肳�ꂽ��ނ�ID�̃A�C�e�����擾�B
	Item::BaseInfo* GetItemInfo(const unsigned int& id, ItemKodeE kode) {
		switch (kode)
		{
		case ItemKodeE::Item:
			return _ItemListVec.at(id).get();
			break;
		case ItemKodeE::Armor:
			return _ArmorList.at(id).get();
			break;
		case ItemKodeE::Weapon:
			return _WeaponList.at(id).get();
			break;
		default:
			char error[256];
			sprintf(error, "�w�肵���A�C�e���R�[�h�������ł��B");
			MessageBoxA(0, error, "�A�C�e���擾���s", MB_ICONWARNING);
			break;
		}
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

private:
	//�A�C�e���̃��X�g�B
	vector<unique_ptr<Item::ItemInfo>> _ItemList[3];

	//�Q�[���Ŏg���A�C�e���̃��X�g�B
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//�Q�[���Ŏg���h��̃��X�g�B
	vector<unique_ptr<Item::ArmorInfo>> _ArmorList;

	//�Q�[���Ŏg������̃��X�g�B
	vector<unique_ptr<Item::WeaponInfo>> _WeaponList;

	static ItemManager* _Instance;
};