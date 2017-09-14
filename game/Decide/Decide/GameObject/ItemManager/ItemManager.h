#pragma once

namespace Item {

	enum class ItemCodeE
	{
		Item = 0,		//アイテム。
		Armor = 1,		//防具。
		Weapon = 2,		//武器。
		Max
	};

	//アイテムと武器と防具の共通項目をまとめた構造体。
	struct BaseInfo :public Noncopyable
	{
		ItemCodeE TypeID;		//種類(アイテムは0)。
		int ID;					//アイテムID。
		char Name[256];			//アイテム名。
		char Description[256];	//アイテムの説名。
		int Value;				//値段。
		int MaxNum;				//限界所持数。
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
		int Def;				//防具を装備した時に上がる防御力。
		int MagicDef;			//魔法攻撃に対する防御力。
	};
	
	//メンバ変数の情報設定。
	static Support::DATARECORD Armordata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct ArmorInfo,TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ArmorInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ArmorInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,Value),		sizeof(int) },
		{ "Def",Support::DataTypeE::INT ,			offsetof(struct ArmorInfo,Def),		sizeof(int) },
		{ "MagicDef",Support::DataTypeE::INT ,		offsetof(struct ArmorInfo,MagicDef),		sizeof(int) },
	};


	//武器の情報をまとめた構造体。
	struct WeaponInfo :public BaseInfo
	{
		int Atk;				//武器を装備した時に上がる攻撃力(*武器種ごとの基準値でランクは考慮していない)。
		int MagicAtk;			//武器を装備した時に上がる魔法攻撃力(*武器種ごとの基準値でランクは考慮していない)。
		int Dex;				//武器を装備した時に上がる器用度(クリティカル発生率)。			
		int CriticalDamage;		//クリティカルが発生した時にダメージ加算率(元のAtkかMagicAtkの値が基準値)。
	};

	//メンバ変数の情報設定。
	static Support::DATARECORD  Weapondata[] =
	{
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct WeaponInfo,TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct WeaponInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Value),		sizeof(int) },
		{ "Atk",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Atk),		sizeof(int) },
		{ "MagicAtk",Support::DataTypeE::INT ,		offsetof(struct WeaponInfo,MagicAtk),		sizeof(int) },
		{ "Dex",Support::DataTypeE::INT ,			offsetof(struct WeaponInfo,Dex),		sizeof(int) },
		{ "CriticalDamage",Support::DataTypeE::INT ,offsetof(struct WeaponInfo,CriticalDamage),		sizeof(int) },

	};
}

//アイテムの情報を管理するクラス。
class ItemManager
{
private:
	ItemManager();
public:

	//アイテム、武器、防具を一括で読み込み。
	void LoadAllItemData();

	//指定された種類とIDのアイテムを取得。
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

	//ゲームで使うアイテムのリスト。
	vector<unique_ptr<Item::ItemInfo>> _ItemListVec;

	//ゲームで使う防具のリスト。
	vector<unique_ptr<Item::ArmorInfo>> _ArmorList;

	//ゲームで使う武器のリスト。
	vector<unique_ptr<Item::WeaponInfo>> _WeaponList;

	static ItemManager* _Instance;
};