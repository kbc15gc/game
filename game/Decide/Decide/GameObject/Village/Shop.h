#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class ImageObject;

namespace
{
	enum ItemType:int
	{
		ITEM,	//�񕜃A�C�e���Ȃǂ̏��Օi
		WEAPON,	//����
		ARMOR,	//�h��
	};
	//�A�C�e���̏��
	struct ItemData
	{
	public:
		int ItemID;		//ID
		ItemType Type;	//�Ȃ�̃A�C�e�����H
		char Name[256];	//�A�C�e����
		char Description[256];//�����B
		int Value; //���l
	};

	//���i���
	struct Product:Noncopyable
	{
	public:
		int ItemID;		//ID
		ItemType Type;	//�Ȃ�̃A�C�e�����H
	};

	const Support::DATARECORD ProductData[] =
	{
		{ "ItemID",Support::DataTypeE::INT , offsetof(struct Product,ItemID),sizeof(int) },
		{ "Type",Support::DataTypeE::INT, offsetof(struct Product,Type),sizeof(int) },
	};
}

namespace
{
	//��
	struct testchar
	{
		char name[256];
	};
}

//���X�B
class Shop:public GameObject
{
public:
	Shop(const char* name);
	~Shop();

	//���X�̏��ǂݍ��݁B
	void LoadShopData();
private:
	//�w����ʕ\��
	void _OpenMenu();
	//�X�^�e�B�b�N�ϐ��̏�����
	void _StaticInit();
private:
	//�V���b�v�̃X�e�[�g
	enum ShopStateE
	{
		CLOSE,		//���Ă���
		SELECT,		//�I�����(���������邩�H)
		BUY,		//�w����ʁB
		SELL,		//�̔���ʁB�Ȃ���������Ȃ��B
	};
	//���i�̕i���낦�B
	vector<Product*> _ProductList;
	//�V���b�v�̃X�e�[�g
	ShopStateE _State;

	//�ǂ����S�������ꏊ�œ����摜���낤����static�ϐ��B
	//�J�[�\���̉摜
	static ImageObject* _Cursor[2];
	//�����E����̉摜
	static ImageObject* _BuySell;
	//�{���̃E�B���h�E
	static ImageObject* _MainWindow;
private:
	
	static vector<testchar*> _ShopNameList;
};