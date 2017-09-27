/**
* ƒAƒCƒeƒ€ƒEƒBƒ“ƒhƒEƒNƒ‰ƒX‚ÌÀ‘•.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"GameObject\Inventory\Inventory.h"
#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include"GameObject\ItemManager\HoldItem\ConsumptionItem.h"

/**
* ‰Šú‰».
*/
void ItemWindow::Awake()
{
	ImageObject* itemWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 9);
	itemWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	itemWindow->SetSize(Vector2(495.0f, 580.0f));
	itemWindow->SetPivot(0.0f, 0.5f);
	itemWindow->transform->SetParent(transform);
	itemWindow->transform->SetLocalPosition(Vector3(0.0f, 47.0f, 0.0f));

	_WindowName = INSTANCE(GameObjectManager)->AddNew<TextObject>("WindowName", 9);
	_WindowName->Initialize(L"", 30.0f);
	_WindowName->SetAnchor(fbText::TextAnchorE::MiddleCenter);
	_WindowName->transform->SetParent(transform);
	_WindowName->transform->SetLocalPosition(Vector3(250.0f, -280.0f, 0.0f));

	for (int i = 0; i < ItemCellSize; i++)
	{
		Item2D* item2D = INSTANCE(GameObjectManager)->AddNew<Item2D>("Item2D", 9);
		item2D->transform->SetParent(itemWindow->transform);
		item2D->transform->SetLocalPosition(Vector3(270.0f, -220.0f + (i * 52.0f), 0.0f));
		_Item2DList.push_back(item2D);
	}

	_SelectCursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 9);
	_SelectCursor->SetTexture(LOADTEXTURE("cursor.png"));
	_SelectCursor->SetSize(Vector2(50.0f, 50.0f));
	Quaternion rot = Quaternion::Identity;
	rot.SetEuler(Vector3(0.0f, 0.0f, -90.0f));
	_SelectCursor->transform->SetLocalRotation(rot);
	_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
	_SelectCursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));

	_EIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("EIconImage", 9);
	_EIconImage->SetTexture(LOADTEXTURE("UI/E.png"));
	_EIconImage->SetSize(Vector2(30, 30));
	_EIconImage->SetActive(false);

	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}


/**
* ‰Šú‰».
*/
void ItemWindow::Init(Item::ItemCodeE code)
{
	_ItemCode = code;

	switch (_ItemCode)
	{
		case Item::ItemCodeE::Item:
			ItemInit();
			break;
		case Item::ItemCodeE::Armor:
			ArmorInit();
			break;
		case Item::ItemCodeE::Weapon:
			WeaponInit();
			break;
	}
}

/**
* XV.
*/
void ItemWindow::Update()
{
	_EIconImage->SetActive(false, true);

	auto& itemList = INSTANCE(Inventory)->GetInventoryList(_ItemCode);
	for (int i = 0; i < ItemCellSize; i++)
	{
		if (itemList.size() > i && itemList[i] != nullptr)
		{
			_Item2DList[i]->SetActive(true, true);
			_Item2DList[i]->SetItemData(itemList[i]);

			if (_ItemCode != Item::ItemCodeE::Item)
			{
				HoldEquipment* item = (HoldEquipment*)itemList[i];
				if (item->GetIsEquip())
				{
					_EIconImage->SetActive(true, false);
					_EIconImage->transform->SetParent(_Item2DList[i]->transform);
					_HoldItem2DList[0]->SetHoldItem(_Item2DList[i]->GetItemData());
				}
			}
		}
		else
		{
			_Item2DList[i]->SetActive(false, true);
		}
	}

	Input();
}

/**
* ƒAƒCƒeƒ€‚Ì‰Šú‰».
*/
void ItemWindow::ItemInit()
{
	_WindowName->SetString(L"ƒAƒCƒeƒ€ˆê——");

	int ParamCount = 7;
	for (int i = 0; i < ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}
	_ParameterRenderList[0]->SetParam("LV", "UI/gem.png", _Player->GetParamPt(CharacterParameter::Param::LV));
	_ParameterRenderList[1]->SetParam("EXP", "UI/S_Light01.png", _Player->GetExpPt());
	_ParameterRenderList[2]->SetParam("HP", "UI/hp.png", _Player->GetParamPt(CharacterParameter::Param::HP), _Player->GetMaxHPPt());
	_ParameterRenderList[3]->SetParam("MP", "UI/mp.png", _Player->GetParamPt(CharacterParameter::Param::MP), _Player->GetMaxMPPt());
	_ParameterRenderList[4]->SetParam("ATK", "UI/S_Buff02.png", _Player->GetParamPt(CharacterParameter::Param::ATK));
	_ParameterRenderList[5]->SetParam("DEF", "UI/S_Buff03.png", _Player->GetParamPt(CharacterParameter::Param::DEF));
	_ParameterRenderList[6]->SetParam("MONEY", "UI/coins.png", INSTANCE(Inventory)->GetPlayerMoneyPt());

	Vector3 pos[4] =
	{
		Vector3(-250.0f,190.0f,0.0f),	//ã
		Vector3(-250.0f,300.0f,0.0f),	//‰º
		Vector3(-220.0f,245.0f,0.0f),	//‰E
		Vector3(-280.0f,245.0f,0.0f),	//¶
	};
	for (int i = 0; i < 4; i++)
	{
		HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
		holdItem->transform->SetParent(transform);
		holdItem->transform->SetLocalPosition(pos[i]);
		holdItem->Init((i % 2 == 1));
		_HoldItem2DList.push_back(holdItem);
	}
}

/**
* •Ší‚Ì‰Šú‰».
*/
void ItemWindow::WeaponInit()
{
	_WindowName->SetString(L"•Šíˆê——");

	int ParamCount = 6;
	for (int i = 0; i < ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}
	_ParameterRenderList[0]->SetParam("LV", "UI/gem.png", _Player->GetParamPt(CharacterParameter::Param::LV));
	_ParameterRenderList[1]->SetParam("EXP", "UI/S_Light01.png", _Player->GetExpPt());
	_ParameterRenderList[2]->SetParam("HP", "UI/hp.png", _Player->GetParamPt(CharacterParameter::Param::HP), _Player->GetMaxHPPt());
	_ParameterRenderList[3]->SetParam("MP", "UI/mp.png", _Player->GetParamPt(CharacterParameter::Param::MP), _Player->GetMaxMPPt());
	_ParameterRenderList[4]->SetParam("ATK", "UI/S_Buff02.png", _Player->GetParamPt(CharacterParameter::Param::ATK));
	_ParameterRenderList[5]->SetParam("DEF", "UI/S_Buff03.png", _Player->GetParamPt(CharacterParameter::Param::DEF));

	HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
	holdItem->transform->SetParent(transform);
	holdItem->transform->SetLocalPosition(Vector3(-400.0f, 300.0f, 0.0f));
	holdItem->Init();
	_HoldItem2DList.push_back(holdItem);
}

/**
* –h‹ï‚Ì‰Šú‰».
*/
void ItemWindow::ArmorInit()
{
	_WindowName->SetString(L"–h‹ïˆê——");

	int ParamCount = 6;
	for (int i = 0; i < ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}
	_ParameterRenderList[0]->SetParam("LV", "UI/gem.png", _Player->GetParamPt(CharacterParameter::Param::LV));
	_ParameterRenderList[1]->SetParam("EXP", "UI/S_Light01.png", _Player->GetExpPt());
	_ParameterRenderList[2]->SetParam("HP", "UI/hp.png", _Player->GetParamPt(CharacterParameter::Param::HP), _Player->GetMaxHPPt());
	_ParameterRenderList[3]->SetParam("MP", "UI/mp.png", _Player->GetParamPt(CharacterParameter::Param::MP), _Player->GetMaxMPPt());
	_ParameterRenderList[4]->SetParam("ATK", "UI/S_Buff02.png", _Player->GetParamPt(CharacterParameter::Param::ATK));
	_ParameterRenderList[5]->SetParam("DEF", "UI/S_Buff03.png", _Player->GetParamPt(CharacterParameter::Param::DEF));

	HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
	holdItem->transform->SetParent(transform);
	holdItem->transform->SetLocalPosition(Vector3(-400.0f, 300.0f, 0.0f));
	holdItem->Init();
	_HoldItem2DList.push_back(holdItem);
}

/**
* “ü—Í.
*/
void ItemWindow::Input()
{
	auto& itemList = INSTANCE(Inventory)->GetInventoryList(_ItemCode);
	int itemCount = 0;
	for (auto& item : itemList)
	{
		if (item != nullptr)
		{
			itemCount += 1;
		}
	}

	if (itemCount <= 0)
	{
		_SelectCursor->SetActive(false, true);
	}
	else
	{
		_SelectCursor->SetActive(true, true);

		static float ChangeTime = 0.5f;
		static float LocalTime = 0.0f;

		Vector2 LStick = XboxInput(0)->GetAnalog(AnalogInputE::L_STICK);
		LStick /= 32767.0f;
		if (LStick.y >= 0.2f)
		{
			if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU))
			{
				_NowSelectItem = max(0, _NowSelectItem - 1);
				_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
			}
			LocalTime += Time::DeltaTime();
			if (LocalTime >= ChangeTime)
			{
				_NowSelectItem = max(0, _NowSelectItem - 1);
				LocalTime = 0.0f;
				ChangeTime = 0.01f;
				_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
			}
		}
		else if (LStick.y <= -0.2f)
		{

			if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
			{
				_NowSelectItem = min(min(ItemCellSize - 1, itemCount - 1), _NowSelectItem + 1);
				_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
			}
			LocalTime += Time::DeltaTime();
			if (LocalTime >= ChangeTime)
			{
				_NowSelectItem = min(min(ItemCellSize - 1, itemCount - 1), _NowSelectItem + 1);
				LocalTime = 0.0f;
				ChangeTime = 0.01f;
				_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
			}
		}
		else
		{
			ChangeTime = 0.5f;
			LocalTime = 0.0f;
		}

		if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
		{
			if (_ItemCode != Item::ItemCodeE::Item)
			{
				_Player->SetEquipment(_Item2DList[_NowSelectItem]->GetItemData());
			}
			else if (_ItemCode == Item::ItemCodeE::Item)
			{
				ConsumptionItem* item = (ConsumptionItem*)_Item2DList[_NowSelectItem]->GetItemData();
				// æŒ?å®šã—ãŸã‚¢ã‚¤ãƒ?ãƒ?ã‚’ä½¿ç”¨ã€?
				item->UseItem();
				// ä½¿ç”¨ã—ãŸã®ã§æ¸›ã‚‰ã™ã€?
				INSTANCE(Inventory)->SubHoldNum(item->GetInfo(),1);
			}
		}
	}
}
