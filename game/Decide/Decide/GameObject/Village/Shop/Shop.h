#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

#include "GameObject\Village\EventManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class ImageObject;
class TextObject;
class IShopState;

namespace
{
	//���i���
	struct Product:Noncopyable
	{
	public:
		int ItemID;		//ID�B
		Item::ItemCodeE Code;	//�A�C�e�������ʂ���R�[�h�B
	};

	const Support::DATARECORD ProductData[] =
	{
		{ "ItemID",Support::DataTypeE::INT , offsetof(struct Product,ItemID),sizeof(int) },
		{ "Code",Support::DataTypeE::INT, offsetof(struct Product,Code),sizeof(int) },
	};
}

namespace
{
	//��
	struct ShopName
	{
		char name[256];
	};
}

//using���c++11����typedef�̂悤�Ɍ^����ς��邱�Ƃ��ł���B
using ShopSPtr = shared_ptr<IShopState>;

//���X�B
class Shop:public GameObject
{
public:
	//�V���b�v�̃X�e�[�g
	enum class ShopStateE
	{
		None = -1,
		Close,		//���Ă���B
		Select,		//�I����ʁB
		Buy,		//�w����ʁB
		Sell,		//�̔���ʁB
		Confirmation,//�m�F��ʁB
	};
public:
	Shop(const char* name);
	~Shop();

	void Awake()override;
	void Update()override;

	//�V���b�v���j���[���J���B
	void OpenShop(const unsigned int& shopID);

	void OnDisable()override
	{
		INSTANCE(EventManager)->NotifyEndEvent();
	}
private:
	//�V���b�v�����B
	void Close();

	//�X�e�[�g�����X�g�ɒǉ��B
	void SetState();
	//���X�̏��ǂݍ��݁B
	void _LoadShopData(const unsigned int& shopID);

	//�X�e�[�g�ύX�B
	void _ChangeState(const ShopStateE state);

	//�����e�L�X�g�ɕ�����ݒ肷��B
	void SetDescriptionText(string text);
	//�{�C�X�̍Đ��B
	void PlayVoice(string wavfilepath);
	//�����̐ݒ�ƃ{�C�X�̍Đ��B
	void SetDescription(string text, string filepath)
	{
		SetDescriptionText(text);
		PlayVoice(filepath);
	}
	//�x����
	void Pay(int money);
private:
	//�ӂ�񂸁B�X�e�[�g�������Ă����B
	friend class ShopS_Close;
	friend class ShopS_Select;
	friend class ShopS_Trade;
	friend class ShopS_Confirmation;

	//�V���b�v�̃X�e�[�g
	ShopStateE _State;
	//�V���b�v�̃X�e�[�g���X�g�B
	vector<ShopSPtr> _StateList;

	//�V���b�v�̖��O
	vector<unique_ptr<ShopName>> _ShopNameList;
	//�A�C�e���̃��X�g�B
	vector<HoldItemBase*> _ItemList;

	//�m�F��Ɏ��s����֐��B
	function<void()> _ShopFunc;

	//�����̃E�B���h�E�̉摜�B
	ImageObject* _DescriptionWindow;
	//�����e�L�X�g�B
	TextObject* _DescriptionText;
	
	//�����̃E�B���h�E�B
	ImageObject* _MoneyWindow;
	//�����̃e�L�X�g�B
	TextObject* _MoneyText;

	//�{�C�X�Đ��B
	SoundSource _Voice;
};