#include "stdafx.h"
#include "Shop.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

Shop::Shop(const char * name):
	GameObject(name)
{
	//スタティック変数の初期化
	_StaticInit();
	//切り替え
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
	//店の商品読み込み
	_LoadShopData(shopID);
	//セレクトメニューを開く
	_ChangeState(ShopStateE::SELECT);
}

void Shop::_LoadShopData(const unsigned int& shopID)
{
	char path[256];
	//読み込むCSV決定。
	sprintf(path, "Asset/Data/ShopData/%s.csv", _ShopNameList.at(shopID));
	//商品の品ぞろえ。
	vector<unique_ptr<Product>> _ProductList;

	//品揃えを読み込み
	Support::LoadCSVData<Product>(path, ProductData, ARRAY_SIZE(ProductData), _ProductList);

	//アイテムの情報を取得
	for(int idx = 0;idx < _ProductList.size();idx++)
	{
		ItemBase::Item::ItemInfo* item = INSTANCE(ItemManager)->GetItem((unsigned int&)_ProductList[idx]->ItemID, (unsigned int)_ProductList[idx]->Type);

		//nullチェック
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
	//出るときに呼び出される処理。
	_Exit(_State);
	//入るときに呼び出される処理。
	_Enter(state);

	_State = state;
}

void Shop::_Enter(const ShopStateE & state)
{
	switch (state)
	{
		//メニューを閉じる
	case Shop::ShopStateE::CLOSE:
		_Update = nullptr;

		_SelectWindow->SetActive(false, true);
		_MainWindow->SetActive(false, true);
		break;
		//セレクトメニュー。
	case Shop::ShopStateE::SELECT:
		_Update = std::bind(&Shop::_SelectUpdate, this);

		_SelectWindow->SetActive(true, true);
		break;
		//購入メニュー。
	case Shop::ShopStateE::BUY:
		//メニューを作成。
		_CreateMenu();
		_Update = std::bind(&Shop::_BuyUpdate, this);

		_MainWindow->SetActive(true, true);
		break;
	case Shop::ShopStateE::SELL:
		_Update = std::bind(&Shop::_SelectUpdate, this);;
		break;
	default:
		break;
	}
}

void Shop::_Exit(const ShopStateE & state)
{
	switch (state)
	{
		//メニューを閉じる
	case Shop::ShopStateE::CLOSE:		
		break;
		//セレクトメニュー。
	case Shop::ShopStateE::SELECT:
		//_SelectWindow->SetActive(false, true);
		break;
		//購入メニュー。
	case Shop::ShopStateE::BUY:
		//メニューを閉じる。
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
	float height = 0.0f;
	_MenuListHeight = 0.0f;
	FOR(i,_ItemList.size())
	{
		//インスタンス化。
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("shopItem", _MainWindow->GetPriorty());
		
		//テキスト設定。
		char t[256];
		sprintf(t, "名前:%s,  値段:%d$", _ItemList[i]->Name, _ItemList[i]->Value);
		text->SetString(t);
		text->SetFontSize(50);
		text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);

		//高さ設定。
		height += text->GetLength().y;
		_MenuListHeight = max(_MenuListHeight, text->GetLength().y);
		//左端+アイコンの横幅。
		float posx = -(_MainWindow->GetSize().x / 2) + _Cursor[1]->GetSize().x;
		text->transform->SetLocalPosition(posx, height, 0);
		text->transform->SetParent(_MainWindow->transform);

		//リストに追加。
		_MenuList.push_back(text);
	}
}

void Shop::_SelectUpdate()
{
	//現在選択している項目
	int max = 2;
	static int select = 0;
	if(KeyBoardInput->isPush(DIK_UP)|| XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU))
	{
		select = select > 0 ? select - 1 : max - 1;
	}
	else if (KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
	{
		select = (select + 1) % max;
	}
	//カーソルをずらす。
	_Cursor[0]->transform->SetLocalPosition(Vector3(-60, 40 + -80 * select, 0));
	
	//決定(仮)
	if(KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		switch (select)
		{
		case 0:
			_ChangeState(Shop::ShopStateE::BUY);
			break;
		case 1:
			_ChangeState(Shop::ShopStateE::SELL);
			break;
		case 2:
			break;
		default:
			break;
		}
	}
	//キャンセル。
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_ChangeState(Shop::ShopStateE::CLOSE);
	}
}

void Shop::_BuyUpdate()
{
	//現在選択している項目
	static int select = 0;
	//項目が変更されたか？
	bool change = false;
	if (KeyBoardInput->isPush(DIK_UP) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU))
	{
		select = (select > 0) ? select - 1 : _MenuList.size() - 1;
		change = true;
	}
	else if (KeyBoardInput->isPush(DIK_DOWN) || XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
	{
		select = (select + 1) % _MenuList.size();
		change = true;
	}
	//変更されたか？
	if (change)
	{
		//カーソルをずらす。
		float posx = -(_MainWindow->GetSize().x / 2) + _Cursor[1]->GetSize().x / 2;
		float posy = _MenuListHeight * (select + 1) + _MenuListHeight*0.5f;
		_Cursor[1]->transform->SetLocalPosition(posx, posy, 0);
		//アイテムの情報を送る。
		_ItemList[select]->Description;
	}


	//決定(仮)
	if (KeyBoardInput->isPush(DIK_P) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//購入確認画面を出す。
	}
	//キャンセル。
	if (KeyBoardInput->isPush(DIK_B) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		_ChangeState(Shop::ShopStateE::SELECT);
	}
}

void Shop::_StaticInit()
{
	//背景
	if (_SelectWindow == nullptr)
	{
		_SelectWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopBuySell", 8);
		_SelectWindow->SetTexture(LOADTEXTURE("ShopSelect.png"));
		_SelectWindow->transform->SetPosition(Vector3(1050, 100, 0));
	}
	//背景
	if (_MainWindow == nullptr)
	{
		_MainWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainWindouw", 8);
		_MainWindow->SetTexture(LOADTEXTURE("window.png"));
		_MainWindow->SetSize(Vector2(800, 600));
		_MainWindow->transform->SetPosition(Vector3(450, 50, 0));
		_MainWindow->SetPivot(Vector2(0.5f, 0.0f));
	}
	//カーソル
	if (_Cursor[0] == nullptr)
	{
		_Cursor[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopCursor", 8);
		_Cursor[0]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[0]->transform->SetParent(_SelectWindow->transform);
		_Cursor[0]->transform->SetLocalPosition(Vector3(-10, 10, 0));
	}
	//カーソル
	if (_Cursor[1] == nullptr)
	{
		_Cursor[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainCursor", 8);
		_Cursor[1]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[1]->transform->SetParent(_MainWindow->transform);
		_Cursor[1]->transform->SetLocalPosition(Vector3(-10, 10, 0));
	}
	//ウィンドウを非アクティブに
	_SelectWindow->SetActive(false, true);
	_MainWindow->SetActive(false, true);
	//ショップ読み込み
	if (_ShopNameList.size() <= 0)
	{
		Support::DATARECORD shopdata[] =
		{
			"ShopName",Support::DataTypeE::STRING,0,sizeof(char) * 256
		};
		Support::LoadCSVData<testchar>("Asset/Data/ShopData/ShopName.csv", shopdata, ARRAY_SIZE(shopdata), _ShopNameList);
	}
}