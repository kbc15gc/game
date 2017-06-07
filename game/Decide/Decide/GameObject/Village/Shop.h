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

	typedef char ShopName[256];
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
	//�V���b�v�̃X�e�[�g
	enum ShopStateE
	{
		CLOSE,		//���Ă���
		SELECT,		//�I�����(���������邩�H)
		BUY,		//�w����ʁB
		SELL,		//�̔���ʁB�Ȃ���������Ȃ��B
	};
public:
	Shop(const char* name);
	~Shop();

	void Update()override;

	//�V���b�v�I�[�v���B
	void OpenShop(const unsigned int& shopID);
private:
	//�w����ʕ\���B
	void _OpenMenu();
	//���X�̏��ǂݍ��݁B
	void _LoadShopData(const unsigned int& shopID);
	//�X�^�e�B�b�N�ϐ��̏������B
	void _StaticInit();

	//�X�e�[�g�ύX�B
	void _ChangeState(const ShopStateE& state);

	//�I����ʂ̍X�V�B
	void _SelectUpdate();
	//�w����ʂ̍X�V�B
	void _BuyUpdate();
private:
	//���i�̕i���낦�B
	vector<Product*> _ProductList;
	//�V���b�v�̃X�e�[�g
	ShopStateE _State;
	//�X�e�[�g�ɂ���ĕς��A�b�v�f�[�g
	std::function<void()> _Update;

	//�ǂ����S�������ꏊ�œ����摜���낤����static�ϐ��B
	//�J�[�\���̉摜
	static ImageObject* _Cursor[2];
	//�����E����̉摜
	static ImageObject* _SelectWindow;
	//�{���̃E�B���h�E
	static ImageObject* _MainWindow;
	//�V���b�v�̖��O
	static vector<testchar*> _ShopNameList;
};