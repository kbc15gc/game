#include "stdafx.h"
#include "Shop.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

#include "GameObject\Village\Shop\HFSM\ShopS_Close.h"
#include "GameObject\Village\Shop\HFSM\ShopS_Select.h"
#include "GameObject\Village\Shop\HFSM\ShopS_Trade.h"
#include "GameObject\Village\Shop\HFSM\ShopS_Confirmation.h"

#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\ItemManager\HoldItem\HoldArmor.h"
#include "GameObject\ItemManager\HoldItem\HoldWeapon.h"

Shop::Shop(const char * name):
	GameObject(name),
	_State(ShopStateE::Select),
	_Voice("Voice")
{
}

Shop::~Shop()
{
}

void Shop::Awake()
{
	//�V���b�v�ǂݍ���
	if (_ShopNameList.size() <= 0)
	{
		Support::DATARECORD shopdata[] =
		{
			"ShopName",Support::DataTypeE::STRING,0,sizeof(char) * 256
		};
		Support::LoadCSVData<ShopName>("Asset/Data/ShopData/ShopName.csv", shopdata, ARRAY_SIZE(shopdata), _ShopNameList);
	}
	//�E�B���h�E�̉摜�B
	_DescriptionWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DescriptionWindow", 8);
	_DescriptionWindow->SetTexture(LOADTEXTURE("window.png"));
	_DescriptionWindow->SetSize(Vector2(800, 200));
	_DescriptionWindow->transform->SetPosition(Vector3(450, 570, 0));
	//�������̃e�L�X�g�B
	_DescriptionText = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", 8);
	_DescriptionText->transform->SetParent(_DescriptionWindow->transform);
	_DescriptionText->transform->SetLocalPosition(Vector3(-_DescriptionWindow->GetSize().x / 2 + 30, -_DescriptionWindow->GetSize().y / 2 + 10, 0));
	_DescriptionText->Initialize(L"TEST", 40);
	_DescriptionText->SetAnchor(fbText::TextAnchorE::UpperLeft);
	
	//�E�B���h�E�̉摜�B
	_MoneyWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DescriptionWindow", 8);
	_MoneyWindow->SetTexture(LOADTEXTURE("window.png"));
	_MoneyWindow->SetSize(Vector2(300, 40));
	_MoneyWindow->transform->SetPosition(Vector3(700, 25, 0));
	//
	_MoneyText = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", 8);
	_MoneyText->transform->SetParent(_MoneyWindow->transform);
	_MoneyText->Initialize(L"0$", 40);
	_MoneyText->transform->SetLocalPosition(Vector3(130, -_MoneyText->GetLength().y, 0));
	_MoneyText->SetAnchor(fbText::TextAnchorE::MiddleRight);
	Pay(0);
	//�X�e�[�g�̏������B
	SetState();

	_ChangeState(Shop::ShopStateE::Close);
}

void Shop::Update()
{
	_StateList.at((int)_State)->Update();
	_Voice.Update();
}

void Shop::OpenShop(const unsigned int & shopID)
{
	if (_State == Shop::ShopStateE::Close)
	{
		//�v���C���[�̂����̓ǂݍ���
		Pay(0);

		//�X�̏��i�ǂݍ���
		_LoadShopData(shopID);
		SetActive(true);
		_DescriptionWindow->SetActive(true, true);
		_MoneyWindow->SetActive(true, true);
		//�Z���N�g���j���[���J��
		_ChangeState(ShopStateE::Select);
	}
}

void Shop::Close()
{
	_DescriptionWindow->SetActive(false, true);
	_MoneyWindow->SetActive(false, true);
	SetActive(false);
}

void Shop::SetState()
{
	_StateList.push_back(shared_ptr<ShopS_Close>(new ShopS_Close(this)));
	_StateList.push_back(shared_ptr<ShopS_Select>(new ShopS_Select(this)));
	auto trade = shared_ptr<ShopS_Trade>(new ShopS_Trade(this));
	_StateList.push_back(trade);
	_StateList.push_back(trade);
	_StateList.push_back(shared_ptr<ShopS_Confirmation>(new ShopS_Confirmation(this)));
}

void Shop::_LoadShopData(const unsigned int& shopID)
{
	//���X�g�̒��g�폜�B
	FOR(i, _ItemList.size())
		INSTANCE(GameObjectManager)->AddRemoveList(_ItemList[i]);
	_ItemList.clear();

	//ID���͈͊O�Ȃ�B
	if(shopID >= _ShopNameList.size())
	{
		FOR(code, Item::ItemCodeE::Max)
		{
			auto maxnum = INSTANCE(ItemManager)->GetMaxID((Item::ItemCodeE)code);
			FOR(id, maxnum)
			{
				HoldItemBase* hitem = HoldItemFactory::CreateItem((Item::ItemCodeE)code, id, false);
				_ItemList.push_back(hitem);
			}
		}
		return;
	}
	char path[256];
	//�ǂݍ���CSV����B
	sprintf(path, "Asset/Data/ShopData/%s.csv", _ShopNameList.at(shopID).get()->name);
	//���i�̕i���낦�B
	vector<unique_ptr<Product>> _ProductList;
	//�i������ǂݍ���
	Support::LoadCSVData<Product>(path, ProductData, ARRAY_SIZE(ProductData), _ProductList);

	//�V���b�v�ɕ��ׂ�A�C�e���쐬�B
	for(int idx = 0;idx < _ProductList.size();idx++)
	{
		HoldItemBase* hitem = HoldItemFactory::CreateItem(_ProductList[idx]->Code,_ProductList[idx]->ItemID, false);
		_ItemList.push_back(hitem);
	}
}

void Shop::_ChangeState(const ShopStateE state)
{
	//���ȑJ�ڂ͂��Ȃ��B
	if (state == _State)
		return;
	//�͈͊O�`�F�b�N�B
	try
	{
		//�X�e�[�g�B
		auto now = _StateList.at((int)_State);
		auto next = _StateList.at((int)state);

		//�o��Ƃ��ɌĂяo����鏈���B
		now->Exit(state);

		auto old = _State;
		//�V�����X�e�[�g��ݒ�B
		_State = state;

		//����Ƃ��ɌĂяo����鏈���B
		next->Enter(old);
	}
	catch (const out_of_range& oor)
	{
		OutputDebugString(oor.what());
	}
}

void Shop::SetDescriptionText(string text)
{
	_DescriptionText->SetText(text.c_str());
}

void Shop::PlayVoice(string wavfilepath)
{
	_Voice.Init(wavfilepath.c_str(), false);
	//�������������̂ŁB
	_Voice.SetVolume(2.0f);
	_Voice.Play(false);
}

void Shop::Pay(int money)
{
	//�����������Z�B
	INSTANCE(Inventory)->SubtractPlayerMoney(money);
	char mo[10];
	//
	sprintf(mo,"%6d$", INSTANCE(Inventory)->GetPlayerMoney());
	_MoneyText->SetText(mo);
}
