#include "stdafx.h"
#include "Shop.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

Shop::Shop(const char * name):
	GameObject(name)
{
	//�X�^�e�B�b�N�ϐ��̏�����
	_StaticInit();
	//�؂�ւ�
	_ChangeState(ShopStateE::CLOSE);
}

Shop::~Shop()
{
}

void Shop::Update()
{
	if (_Update)
		_Update();
}

void Shop::OpenShop(const unsigned int & shopID)
{
	//�X�̏��i�ǂݍ���
	_LoadShopData(shopID);
	//�Z���N�g���j���[���J��
	_ChangeState(ShopStateE::SELECT);
}

void Shop::_LoadShopData(const unsigned int& shopID)
{
	char path[256];
	//�ǂݍ���CSV����B
	sprintf(path, "Asset/Data/ShopData/%s.csv", _ShopNameList.at(shopID));
	//���i�̕i���낦�B
	vector<unique_ptr<Product>> _ProductList;

	//�i������ǂݍ���
	Support::LoadCSVData<Product>(path, ProductData, ARRAY_SIZE(ProductData), _ProductList);

	//�A�C�e���̏����擾
	for(int idx = 0;idx < _ProductList.size();idx++)
	{
		Item::ItemInfo* item = INSTANCE(ItemManager)->GetItem((unsigned int&)_ProductList[idx]->ItemID, (unsigned int)_ProductList[idx]->Type);

		//null�`�F�b�N
		if (item)
			_ItemList.push_back(item);
	}
}

void Shop::_CloseMenu()
{
	FOR(i, _MenuList.size())
	{
		INSTANCE(GameObjectManager)->AddRemoveList(_MenuList[i]);
	}
	_MenuList.clear();
}

void Shop::_ChangeState(const ShopStateE & state)
{
	//�o��Ƃ��ɌĂяo����鏈���B
	_Exit(_State);
	//����Ƃ��ɌĂяo����鏈���B
	_Enter(state);

	_State = state;
}

void Shop::_Enter(const ShopStateE & state)
{
	switch (state)
	{
		//���j���[�����
	case Shop::ShopStateE::CLOSE:
		_Update = nullptr;

		_SelectWindow->SetActive(false, true);
		_MainWindow->SetActive(false, true);
		break;
		//�Z���N�g���j���[�B
	case Shop::ShopStateE::SELECT:
		_Update = std::bind(&Shop::_SelectUpdate, this);

		_SelectWindow->SetActive(true, true);
		break;
		//�w�����j���[�B
	case Shop::ShopStateE::BUY:
		//���j���[���쐬�B
		_CreateMenu();
		_Update = std::bind(&Shop::_BuyUpdate, this);

		_MainWindow->SetActive(true, true);
		break;
	case Shop::ShopStateE::SELL:
		_Update = nullptr;
		break;
	default:
		break;
	}
}

void Shop::_Exit(const ShopStateE & state)
{
	switch (state)
	{
		//���j���[�����
	case Shop::ShopStateE::CLOSE:		
		break;
		//�Z���N�g���j���[�B
	case Shop::ShopStateE::SELECT:
		//_SelectWindow->SetActive(false, true);
		break;
		//�w�����j���[�B
	case Shop::ShopStateE::BUY:
		//���j���[�����B
		_CloseMenu();
		_MainWindow->SetActive(false, true);
		break;
	case Shop::ShopStateE::SELL:
		_Update = nullptr;
		break;
	default:
		break;
	}
}

void Shop::_CreateMenu()
{
	float Y = 0.0f;
	FOR(i,_ItemList.size())
	{
		//�C���X�^���X���B
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _MainWindow->GetPriorty());
		text->transform->SetParent(_MainWindow->transform);
		//�����ݒ�B
		char t[512];
		sprintf(t, "%s", _ItemList[i]->Name);
		text->SetString(t);
		text->SetFontSize(40.0f);
		text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);
		//�����ݒ�B
		Y += text->GetLength().y;
		text->transform->SetLocalPosition(100 + -(_MainWindow->GetSize().x / 2), Y, 0);
		//���X�g�ɒǉ��B
		_MenuList.push_back(text);
	}
}

void Shop::_SelectUpdate()
{
	//���ݑI�����Ă��鍀��
	static int select = 0;
	if(KeyBoardInput->isPush(DIK_UP)|| XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU))
	{
		select = select > 0 ? select - 1 : 1;
	}
	else if (KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
	{
		select = (select + 1) % 2;
	}
	//�J�[�\�������炷�B
	_Cursor[0]->transform->SetLocalPosition(Vector3(-60, 40 + -80 * select, 0));
	
	//����(��)
	if(KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		if(select)
		{
			_ChangeState(Shop::ShopStateE::BUY);
		}else
		{
			_ChangeState(Shop::ShopStateE::SELL);
		}
	}
	//�L�����Z���B
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_ChangeState(Shop::ShopStateE::CLOSE);
	}
}

void Shop::_BuyUpdate()
{
	//���ݑI�����Ă��鍀��
	static int select = 0;
	if (KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU))
	{
		select = select > 0 ? select - 1 : 1;
	}
	else if (KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
	{
		select = (select + 1) % _MenuList.size();
	}
	//�J�[�\�������炷�B
	_Cursor[1]->transform->SetLocalPosition(Vector3(-(_MainWindow->GetSize().x / 2), 40 + -80 * select, 0));
	//�L�����Z���B
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_ChangeState(Shop::ShopStateE::SELECT);
	}
}

void Shop::_StaticInit()
{
	//�w�i
	if (_SelectWindow == nullptr)
	{
		_SelectWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopBuySell", 8);
		_SelectWindow->SetTexture(LOADTEXTURE("ShopSelect.png"));
		_SelectWindow->transform->SetPosition(Vector3(800, 200, 0));
	}
	//�w�i
	if (_MainWindow == nullptr)
	{
		_MainWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainWindouw", 8);
		_MainWindow->SetTexture(LOADTEXTURE("window.png"));
		_MainWindow->SetSize(Vector2(800, 600));
		_MainWindow->transform->SetPosition(Vector3(600, 100, 0));
		_MainWindow->SetPivot(Vector2(0.5f, 0.0f));
	}
	//�J�[�\��
	if (_Cursor[0] == nullptr)
	{
		_Cursor[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopCursor", 8);
		_Cursor[0]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[0]->transform->SetParent(_SelectWindow->transform);
		_Cursor[0]->transform->SetLocalPosition(Vector3(-10, 10, 0));
	}
	//�J�[�\��
	if (_Cursor[1] == nullptr)
	{
		_Cursor[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainCursor", 8);
		_Cursor[1]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[1]->transform->SetParent(_MainWindow->transform);
		_Cursor[1]->transform->SetLocalPosition(Vector3(-10, 10, 0));
	}
	//�E�B���h�E���A�N�e�B�u��
	_SelectWindow->SetActive(false, true);
	_MainWindow->SetActive(false, true);
	//�V���b�v�ǂݍ���
	if (_ShopNameList.size() <= 0)
	{
		Support::DATARECORD shopdata[] =
		{
			"ShopName",Support::DataTypeE::STRING,0,sizeof(char) * 256
		};
		Support::LoadCSVData<testchar>("Asset/Data/ShopData/ShopName.csv", shopdata, ARRAY_SIZE(shopdata), _ShopNameList);
	}
}