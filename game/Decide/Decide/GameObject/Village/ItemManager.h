#pragma once

namespace Item
{
	//�A�C�e���̏����܂Ƃ߂��\���́B
	struct ItemInfo :Noncopyable
	{
		int ID;					//�A�C�e��ID
		char Name[256];			//�A�C�e����
		char Description[256];	//�A�C�e���̐��B
		int Value;				//�l�i�H
		int ATK;				//���ꂼ��̒l�B(��)
	};

	//�����o�ϐ��̏��ݒ�
	static Support::DATARECORD Itemdata[] =
	{
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,ID),			sizeof(int) },
		{ "Name",Support::DataTypeE::STRING,		offsetof(struct ItemInfo,Name),			sizeof(char) * 256 },
		{ "Description",Support::DataTypeE::STRING, offsetof(struct ItemInfo,Description),	sizeof(char) * 256 },
		{ "Value",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,Value),		sizeof(int) },
		{ "ATK",Support::DataTypeE::INT ,			offsetof(struct ItemInfo,ATK),			sizeof(int) },
	};
}

//�A�C�e���̏����Ǘ�����N���X
class ItemManager
{
public:
	ItemManager();

	//�A�C�e���̏���ǂݍ��݁B
	void LoadItemData();

	Item::ItemInfo* GetItem(const unsigned int& id,const unsigned int& type)
	{
		//�͈̓`�F�b�N
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
	//�A�C�e���̃��X�g
	vector<Item::ItemInfo*> _ItemList[3];
	
	static ItemManager* _Instance;
};