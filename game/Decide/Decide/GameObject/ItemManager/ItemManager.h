#pragma once
#include "GameObject\Component\CharacterParameter.h"

namespace Item {

	enum class ItemCodeE
	{
		Item = 0,		//�A�C�e���B
		Armor = 1,		//�h��B
		Weapon = 2,		//����B
		Max
	};

	//�A�C�e���ƕ���Ɩh��̋��ʍ��ڂ��܂Ƃ߂��\���́B
	struct BaseInfo :public Noncopyable
	{
		ItemCodeE TypeID;		//���(�A�C�e����0)�B
		int ID;					//�A�C�e��ID�B
		char Name[256];			//�A�C�e�����B
		char Description[256];	//�A�C�e���̐����B
		int Value;				//�l�i�B
		char filePath[256];		//�A�C�e���̃A�C�R��(UI�ȂǂŎg�p)�B
	};
	//�A�C�e���̏����܂Ƃ߂��\���́B
	struct ItemInfo :public BaseInfo
	{
		int type;	// �A�C�e���̎��(�񕜂�0�A�o�t��1�A�f�o�t��2)�B
		int effectValue[CharacterParameter::Param::MAX];	// �p�����[�^���Z�l(�񕜃A�C�e���Ȃ�HP��+�̒l�A�f�o�t�Ȃ�-�̒l����������)�B
		int rangeStrength;		// ���ʔ͈͂̋��x(0�ň�l�A1�ŎO�l�A2�Ōܐl�A3�őS��)�B
		float time;				// ���ʎ���(�b)�B
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD Itemdata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct ItemInfo,TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ItemInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ItemInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT, offsetof(struct ItemInfo,Value),	sizeof(int)},
		{ "filePath",Support::DataTypeE::STRING, offsetof(struct ItemInfo,filePath),	sizeof(char) * 256 },
		{ "type",Support::DataTypeE::INT, offsetof(struct ItemInfo,type),	sizeof(int) },
		{ "effectValue",Support::DataTypeE::INTARRAY, offsetof(struct ItemInfo,effectValue),	sizeof(ItemInfo::effectValue) },
		{ "rangeStrength",Support::DataTypeE::INT, offsetof(struct ItemInfo,rangeStrength),	sizeof(int) },
		{ "time",Support::DataTypeE::FLOAT ,			offsetof(struct ItemInfo,time),		sizeof(float) },
	};

	// ����A�C�e���̈�g���̏�������B
	static int holdMax = 99;


	//�h��̏����܂Ƃ߂��\���́B
	struct ArmorInfo :public BaseInfo
	{
		int Def;				//�h��𑕔��������ɏオ��h��́B
		int MagicDef;			//���@�U���ɑ΂���h��́B
	};
	
	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD Armordata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct ArmorInfo,TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ArmorInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ArmorInfo,Description),	sizeof(char) * 256 },
		{ "filePath",Support::DataTypeE::STRING, offsetof(struct ArmorInfo,filePath),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,Value),		sizeof(int) },
		{ "Def",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,Def),		sizeof(int) },
		{ "MagicDef",Support::DataTypeE::INT ,		offsetof(struct ArmorInfo,MagicDef),		sizeof(int) },
	};


	//����̏����܂Ƃ߂��\���́B
	struct WeaponInfo :public BaseInfo
	{
		int Atk;				//����𑕔��������ɏオ��U����(*����킲�Ƃ̊�l�Ń����N�͍l�����Ă��Ȃ�)�B
		int MagicAtk;			//����𑕔��������ɏオ�閂�@�U����(*����킲�Ƃ̊�l�Ń����N�͍l�����Ă��Ȃ�)�B
		int Dex;				//����𑕔��������ɏオ���p�x(�N���e�B�J��������)�B			
		int CriticalDamage;		//�N���e�B�J���������������Ƀ_���[�W���Z��(����Atk��MagicAtk�̒l����l)�B
	};

	//�����o�ϐ��̏��ݒ�B
	static Support::DATARECORD  Weapondata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct WeaponInfo,TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct WeaponInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,Description),	sizeof(char) * 256 },
		{ "filePath",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,filePath),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Value),		sizeof(int) },
		{ "Atk",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Atk),		sizeof(int) },
		{ "MagicAtk",Support::DataTypeE::INT ,		offsetof(struct WeaponInfo,MagicAtk),		sizeof(int) },
		{ "Dex",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Dex),		sizeof(int) },
		{ "CriticalDamage",Support::DataTypeE::INT ,offsetof(struct WeaponInfo,CriticalDamage),		sizeof(int) },

	};
}

//�A�C�e���̏����Ǘ�����N���X�B
class ItemManager
{
private:
	ItemManager();
public:

	//�A�C�e���A����A�h����ꊇ�œǂݍ��݁B
	void LoadAllItemData();

	//�w�肳�ꂽ��ނ�ID�̃A�C�e�����擾�B
	Item::BaseInfo* GetItemInfo(const unsigned int& id, Item::ItemCodeE code) {
		switch (code)
		{
		case Item::ItemCodeE::Item:
			return _ItemListVec.at(id).get();
			break;
		case  Item::ItemCodeE::Armor:
			return _ArmorList.at(id).get();
			break;
		case  Item::ItemCodeE::Weapon:
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

	//�Q�[���Ŏg���A�C�e���̃��X�g�B
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//�Q�[���Ŏg���h��̃��X�g�B
	vector<unique_ptr<Item::ArmorInfo>> _ArmorList;

	//�Q�[���Ŏg������̃��X�g�B
	vector<unique_ptr<Item::WeaponInfo>> _WeaponList;

	static ItemManager* _Instance;
};