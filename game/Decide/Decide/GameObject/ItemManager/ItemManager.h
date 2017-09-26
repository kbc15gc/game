#pragma once
#include "GameObject\Component\CharacterParameter.h"

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
		char filePath[256];		//アイテムのアイコン(UIなどで使用)。
	};
	//アイテムの情報をまとめた構造体。
	struct ItemInfo :public BaseInfo
	{
		int type;	// アイテムの種類(回復が0、バフが1、デバフが2)。
		int effectValue[CharacterParameter::Param::MAX];	// パラメータ加算値(回復アイテムならHPに+の値、デバフなら-の値を書き込む)。
		int rangeStrength;		// 効果範囲の強度(0で一人、1で三人、2で五人、3で全体)。
		float time;				// 効果時間(秒)。
	};

	//メンバ変数の情報設定。
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

	// 消費アイテムの一枠分の所持上限。
	static int holdMax = 99;


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
		{ "filePath",Support::DataTypeE::STRING, offsetof(struct ArmorInfo,filePath),	sizeof(char) * 256 },
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
		{ "filePath",Support::DataTypeE::STRING, offsetof(struct WeaponInfo,filePath),	sizeof(char) * 256 },
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