#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\ItemManager.h"

class ImageObject;
class TextObject;

namespace
{
	enum ItemType :int
	{
		ITEM,	//�񕜃A�C�e���Ȃǂ̏��Օi
		WEAPON,	//����
		ARMOR,	//�h��
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

	//�V���b�v���j���[���J���B
	void OpenShop(const unsigned int& shopID);
private:
	//���X�̏��ǂݍ��݁B
	void _LoadShopData(const unsigned int& shopID);

	//���j���[�����B
	void _CloseMenu();

	//�X�e�[�g�ύX�B
	void _ChangeState(const ShopStateE& state);
	//����Ƃ��̏����B
	void _Enter(const ShopStateE& state);
	//�o��Ƃ��̏����B
	void _Exit(const ShopStateE& state);
	
	//���j���[�����B
	void _CreateMenu();

	//�I����ʂ̍X�V�B
	void _SelectUpdate();
	//�w����ʂ̍X�V�B
	void _BuyUpdate();

	//�X�^�e�B�b�N�ϐ��̏������B
	void _StaticInit();
private:
	//�V���b�v�̖��O
	vector<unique_ptr<testchar>> _ShopNameList;
	//�A�C�e���̃��X�g�B
	vector<Item::ItemInfo*> _ItemList;
	//�V���b�v�̃X�e�[�g
	ShopStateE _State;
	//�X�e�[�g�ɂ���ĕς��A�b�v�f�[�g
	std::function<void()> _Update;

	//�J�[�\���̉摜
	ImageObject* _Cursor[2];
	//�����E����̉摜
	ImageObject* _SelectWindow;
	//�{���̃E�B���h�E
	ImageObject* _MainWindow;
	//���j���[�̃e�L�X�g
	vector<TextObject*> _MenuList;
};