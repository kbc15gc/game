#pragma once

namespace Item {

	struct BaseInfo :public Noncopyable
	{
		int TypeID;				//種類(アイテムは0)。
		int ID;					//アイテムID。
		char Name[256];			//アイテム名。
		char Description[256];	//アイテムの説。
		int Value;				//値段。
	};
	//アイテムの情報をまとめた構造体。
	struct ItemInfo :public BaseInfo
	{
		
		int Recovery;			//薬草を使った時の回復量。
		int AtkBuff;			//薬草を使った時の攻撃力の上昇量。
		int DefBuff;			//薬草を使った時の防御力の上昇量。
		int SpeedBuff;			//薬草を使った時の移動速度の上昇量。
	};

	//メンバ変数の情報設定。
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

	//防具の情報をまとめた構造体。
	struct ArmorInfo :public BaseInfo
	{
		//int TypeID;				//種類(防具は1)。
		//int ID;					//防具ID。
		//char Name[256];			//防具名。
		//char Description[256];	//防具の説。
		//int Value;				//値段。
		int ATK;				//防具を装備した時に上がる攻撃力。
		int DEF;				//防具を装備した時に上がる防御力。
	};

	//メンバ変数の情報設定。
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


	//武器の情報をまとめた構造体。
	struct WeaponInfo :public BaseInfo
	{
		//int TypeID;				//種類(武器は2)。
		//int ID;					//武器ID。
		//char Name[256];			//武器名。
		//char Description[256];	//武器の説。
		//int Value;				//値段。
		int ATK;				//武器を装備した時に上がる攻撃力。
	};

	//メンバ変数の情報設定。
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

//アイテムの情報を管理するクラス。
class ItemManager
{
private:
	ItemManager();
public:
	//ItemManager();

	enum class ItemKodeE
	{
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

	//指定された種類とIDのアイテムを取得。
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
			sprintf(error, "指定したアイテムコードが無効です。");
			MessageBoxA(0, error, "アイテム取得失敗", MB_ICONWARNING);
			break;
		}
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

private:
	//アイテムのリスト。
	vector<unique_ptr<Item::ItemInfo>> _ItemList[3];

	//ゲームで使うアイテムのリスト。
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//ゲームで使う防具のリスト。
	vector<unique_ptr<Item::ArmorInfo>> _ArmorList;

	//ゲームで使う武器のリスト。
	vector<unique_ptr<Item::WeaponInfo>> _WeaponList;

	static ItemManager* _Instance;
};