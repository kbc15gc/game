#pragma once

namespace Item
{
	//アイテムの情報をまとめた構造体。
	struct ItemInfo :Noncopyable
	{
		int ID;					//アイテムID。
		char Name[256];			//アイテム名。
		char Description[256];	//アイテムの説。
		int Value;				//値段。
	};

	//メンバ変数の情報設定。
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
	//防具の情報をまとめた構造体。
	struct ArmorInfo :Noncopyable
	{
		int ID;					//防具ID。
		char Name[256];			//防具名。
		char Description[256];	//防具の説。
		int Value;				//値段。
		int ATK;				//防具を装備した時に上がる攻撃力。
		int DEF;				//防具を装備した時に上がる防御力。
	};

	//メンバ変数の情報設定
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
	//武器の情報をまとめた構造体。
	struct WeaponInfo :Noncopyable
	{
		int ID;					//武器ID。
		char Name[256];			//武器名。
		char Description[256];	//武器の説。
		int Value;				//値段。
		int ATK;				//武器を装備した時に上がる攻撃力。
	};

	//メンバ変数の情報設定
	static Support::DATARECORD  Weapondata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct WeaponInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ATK),		sizeof(int) },
	};
}

//アイテムの情報を管理するクラス
class ItemManager
{
public:
	ItemManager();

	void LoadAllData();

	//アイテムの情報を読み込み。
	void LoadItemData();

	//防具の情報を読み込み。
	void LoadArmorData();

	//武器の情報を読み込み。
	void LoadWeaponData();

	Item::ItemInfo* GetItem(const unsigned int& id,const unsigned int& type)
	{
		//範囲チェック
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
			//アイテムの情報を読み込み。
			_Instance->LoadItemData();
		}
		return _Instance;
	}
private:
	//アイテムのリスト
	vector<unique_ptr<Item::ItemInfo>> _ItemList[3];

	//アイテムのリスト
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//防具のリスト
	vector<unique_ptr<Armor::ArmorInfo>> _ArmorList;

	//武器のリスト
	vector<unique_ptr<Weapon::WeaponInfo>> _WeaponList;
	
	static ItemManager* _Instance;
};