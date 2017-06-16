#pragma once

namespace Item
{
	//アイテムの情報をまとめた構造体。
	struct ItemInfo :Noncopyable
	{
		int ID;					//アイテムID
		char Name[256];			//アイテム名
		char Description[256];	//アイテムの説。
		int Value;				//値段？
		int ATK;				//それぞれの値。(仮)
	};

	//メンバ変数の情報設定
	static Support::DATARECORD Itemdata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ItemInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ItemInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,ATK),			sizeof(int) },
	};
}

//アイテムの情報を管理するクラス
class ItemManager
{
public:
	ItemManager();

	//アイテムの情報を読み込み。
	void LoadItemData();

	Item::ItemInfo* GetItem(const unsigned int& id,const unsigned int& type)
	{
		//範囲チェック
		if (type < 2)
			return _ItemList[type].at(id);
		else
			return nullptr;
	}
	
	static ItemManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new ItemManager();
		}
		return _Instance;
	}
private:
	//アイテムのリスト
	vector<Item::ItemInfo*> _ItemList[3];
	
	static ItemManager* _Instance;
};