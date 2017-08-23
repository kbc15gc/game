#pragma once
#include<tuple>
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

	//メンバ変数の情報設定。
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

	//メンバ変数の情報設定。
	static Support::DATARECORD  Weapondata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct WeaponInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ATK),		sizeof(int) },
	};
}

//アイテムの情報を管理するクラス。
class ItemManager
{
public:
	ItemManager();

	enum class ItemKodeE
	{
		Invalid = -1,	//無効。
		Item = 0,		//アイテム。
		Armor = 1,		//防具。
		Weapon = 2,		//武器。
	};

	//アイテム、武器、防具を一括で読み込み。
	void LoadAllItemData();

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
			_Instance->LoadAllItemData();
		}
		return _Instance;
	}


	//プレイヤーの所持品にアイテムを追加。第1引数:追加するアイテムの種類、第2引数:アイテムID、第3引数:名前、第4引数:アイテムの説明、第5引数:値段、、第6引数:攻撃力、第7引数:防御力。
	void AddPlayerItemList(ItemKodeE kode, int* id = nullptr, const char* name = nullptr, const char* des = nullptr, int* value = nullptr, int* atk = nullptr, int* def = nullptr);

	//プレイヤーが所持している防具のリストを取得。
	//Armor::ArmorInfo *GetPlayerAromorInfo();


private:
	//アイテムのリスト。
	vector<unique_ptr<Item::ItemInfo>> _ItemList[3];

	//アイテムのリスト。
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//防具のリスト。
	vector<unique_ptr<Armor::ArmorInfo>> _ArmorList;

	//武器のリスト。
	vector<unique_ptr<Weapon::WeaponInfo>> _WeaponList;

	//プレイヤーが所持しているアイテムのリスト。
	Item::ItemInfo _PlayerItemList[5];

	//プレイヤーが所持している防具のリスト。
	Armor::ArmorInfo _PlayerArmorList[5];

	Armor::ArmorInfo arm[2];

	//プレイヤーが所持している武器のリスト。
	Weapon::WeaponInfo _PlayerWeaponList[5];

	//所持品を格納するリストの最大数。
	int ListNum = 5;

	//プレイヤーが持っているアイテム、防具、武器の数。
	int ItemCounter, ArmorCounter, WeapoCounter = 0;
	
	static ItemManager* _Instance;
};