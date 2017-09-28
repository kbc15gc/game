/**
* アイテムウィンドウクラスの実装.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"GameObject\Inventory\Inventory.h"
#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include"GameObject\ItemManager\HoldItem\ConsumptionItem.h"

/**
* 初期化.
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
	_WindowName->transform->SetLocalPosition(Vector3(250.0f, -235.0f, 0.0f));

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
* 初期化.
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
* アイテムの初期化.
*/
void ItemWindow::ItemInit()
{
	_WindowName->SetText(L"アイテム一覧");

	// ステータス表示作成。
	_CreateCIShowStatus();

	Vector3 pos[4] =
	{
		Vector3(-250.0f,190.0f,0.0f),	//上
		Vector3(-250.0f,300.0f,0.0f),	//下
		Vector3(-220.0f,245.0f,0.0f),	//右
		Vector3(-280.0f,245.0f,0.0f),	//左
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
* 武器の初期化.
*/
void ItemWindow::WeaponInit()
{
	_WindowName->SetText(L"武器一覧");

	_CreateWIShowStatus();

	HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
	holdItem->transform->SetParent(transform);
	holdItem->transform->SetLocalPosition(Vector3(-400.0f, 300.0f, 0.0f));
	holdItem->Init();
	_HoldItem2DList.push_back(holdItem);
}

/**
* 防具の初期化.
*/
void ItemWindow::ArmorInit()
{
	_WindowName->SetText(L"防具一覧");

	_CreateAIShowStatus();

	HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
	holdItem->transform->SetParent(transform);
	holdItem->transform->SetLocalPosition(Vector3(-400.0f, 300.0f, 0.0f));
	holdItem->Init();
	_HoldItem2DList.push_back(holdItem);
}

/**
* 更新.
*/
void ItemWindow::Update()
{
	_ConfigParamRender();

	_EIconImage->SetActive(false, true);

	auto& itemList = INSTANCE(Inventory)->GetInventoryList(_ItemCode);
	for (int i = 0; i < ItemCellSize; i++)
	{
		if (itemList.size() > i + _StartLoadCount && itemList[i + _StartLoadCount] != nullptr)
		{
			_Item2DList[i]->SetActive(true, true);
			_Item2DList[i]->SetItemData(itemList[i + _StartLoadCount]);

			if (_ItemCode != Item::ItemCodeE::Item)
			{
				HoldEquipment* item = (HoldEquipment*)itemList[i + _StartLoadCount];
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
* 入力.
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
				if (_NowSelectItem <= 0)
				{
					if (_StartLoadCount > 0)
					{
						_StartLoadCount--;
					}
					else
					{
						_StartLoadCount = max(0, itemCount - ItemCellSize);
						_NowSelectItem = min(ItemCellSize - 1, itemCount - 1);
					}
				}
				else
				{
					_NowSelectItem = max(0, _NowSelectItem - 1);
				}
				_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
			}
			LocalTime += Time::DeltaTime();
			if (LocalTime >= ChangeTime)
			{
				if (_NowSelectItem <= 0)
				{
					if (_StartLoadCount > 0)
					{
						_StartLoadCount--;
					}
					else
					{
						_StartLoadCount = max(0, itemCount - ItemCellSize);
						_NowSelectItem = min(ItemCellSize - 1, itemCount - 1);
					}
				}
				else
				{
					_NowSelectItem = max(0, _NowSelectItem - 1);
				}
				LocalTime = 0.0f;
				ChangeTime = 0.01f;
				_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
			}
		}
		else if (LStick.y <= -0.2f)
		{

			if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD))
			{
				if (_NowSelectItem >= ItemCellSize - 1)
				{
					if (_NowSelectItem + _StartLoadCount < itemCount - 1)
					{
						_StartLoadCount++;
					}
					else
					{
						_StartLoadCount = 0;
						_NowSelectItem = 0;
					}
				}
				else if (_NowSelectItem >= itemCount - 1)
				{
					_NowSelectItem = 0;
				}
				else
				{
					_NowSelectItem = min(min(ItemCellSize - 1, itemCount - 1), _NowSelectItem + 1);
				}
				_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
			}
			LocalTime += Time::DeltaTime();
			if (LocalTime >= ChangeTime)
			{
				if (_NowSelectItem >= ItemCellSize - 1)
				{
					if (_NowSelectItem + _StartLoadCount < itemCount - 1)
					{
						_StartLoadCount++;
					}
					else
					{
						_StartLoadCount = 0;
						_NowSelectItem = 0;
					}
				}
				else if (_NowSelectItem >= itemCount - 1)
				{
					_NowSelectItem = 0;
				}
				else
				{
					_NowSelectItem = min(min(ItemCellSize - 1, itemCount - 1), _NowSelectItem + 1);
				}
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
				item->UseItem();
				INSTANCE(Inventory)->SubHoldNum(item->GetInfo(),1);
				itemCount = 0;
				for (auto& item : itemList)
				{
					if (item != nullptr)
					{
						itemCount += 1;
					}
				}
				if (_StartLoadCount > 0 && ItemCellSize + _StartLoadCount > itemCount)
				{
					//表示位置を一個さげる.
					_StartLoadCount--;
				}
				else if (_NowSelectItem >= itemCount)
				{
					//選択位置を一個下げる.
					_NowSelectItem = max(0, _NowSelectItem - 1);
				}
				if (itemCount <= 0)
				{
					_SelectCursor->transform->SetParent(nullptr);
					_SelectCursor->SetActive(false, true);
				}
				else
				{
					_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
				}
			}
		}
	}
}

void ItemWindow::_CreateCIShowStatus()
{
	CIShowStatus ParamCount = CIShowStatus::MAX;
	Transform* work = transform;
	Vector3 pos = Vector3(-280.0f, -270.0f, 0.0f);
	for (int i = 0; i < static_cast<int>(ParamCount); i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(work);
		pr->transform->SetLocalPosition(pos + Vector3(0.0f,40.0f, 0.0f));
		_ParameterRenderList.push_back(pr);
		work = pr->transform;
		pos = Vector3::zero;
	}
	// レベルのパラメータは真横に表示。
	_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->GetParamTextPos() + Vector3(-270, 0.0f, 0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->GetIconObject()->transform->SetLocalPosition(_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->GetIconObject()->transform->GetLocalPosition() + Vector3(0.0f, 5.0f, 0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->transform->SetLocalPosition(_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->transform->GetLocalPosition() + Vector3(0.0f,10.0f,0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->GetParamTextPos() + Vector3(-270, 0.0f, 0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->GetParamTextPos() + Vector3(-270, 0.0f, 0.0f));

	_ExpBar = AddComponent<ParameterBar>();
	vector<BarColor> barColor;
	barColor.push_back(BarColor::Yellow);
	_ExpBar->Create(barColor, static_cast<float>(_Player->GetNextLevelExp()), static_cast<float>(_Player->GetExp()), false, false, _ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->transform, Vector3(50.0f, 40.0f, 0.0f), Vector2(1.0f,0.5f), 8,true);

	_HpBar = AddComponent<ParameterBar>();
	barColor.clear();
	barColor.push_back(BarColor::Green);
	_HpBar->Create(barColor, static_cast<float>(_Player->GetMaxHP()), static_cast<float>(_Player->GetParam(CharacterParameter::Param::HP)), false, false, _ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->transform, Vector3(50.0f,20.0f, 0.0f), Vector2(1.0f,0.7f), 8, true);

	_MpBar = AddComponent<ParameterBar>();
	barColor.clear();
	barColor.push_back(BarColor::Blue);
	_MpBar->Create(barColor, static_cast<float>(_Player->GetMaxMP()), static_cast<float>(_Player->GetParam(CharacterParameter::Param::MP)), false, false, _ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->transform, Vector3(50.0f, 20.0f, 0.0f), Vector2(1.0f,0.7f), 8, true);

	_ConfigParamRender();
}

/**
* 武器アイテムの表示ステータスの作成.
*/
void ItemWindow::_CreateWIShowStatus()
{
	WIShowStatus ParamCount = WIShowStatus::MAX;
	for (int i = 0; i < (int)ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}

	_ConfigParamRender();
}

/**
* 防具アイテムの表示ステータスの作成.
*/
void ItemWindow::_CreateAIShowStatus()
{
	AIShowStatus ParamCount = AIShowStatus::MAX;
	for (int i = 0; i < (int)ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}

	_ConfigParamRender();
}

void ItemWindow::_ConfigParamRender() 
{

	switch (_ItemCode)
	{
		case Item::ItemCodeE::Item:
		{
			int playerLevel = _Player->GetParam(CharacterParameter::Param::LV);
			_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->SetParam("LV", "UI/gem.png", playerLevel, fbText::TextAnchorE::MiddleLeft, 0, INT_MIN, 50.0f, Vector2(40.0f, 40.0f));
			_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->SetParam("HP", "UI/hp.png", _Player->GetParam(CharacterParameter::Param::HP), fbText::TextAnchorE::MiddleLeft, 0, _Player->GetMaxHP());
			_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->SetParam("MP", "UI/mp.png", _Player->GetParam(CharacterParameter::Param::MP), fbText::TextAnchorE::MiddleLeft, 0, _Player->GetMaxMP());
			_ParameterRenderList[static_cast<int>(CIShowStatus::ATK)]->SetParam("ATK", "UI/S_Buff02.png", _Player->GetParam(CharacterParameter::Param::ATK), fbText::TextAnchorE::MiddleRight, _Player->GetBuffParam(CharacterParameter::Param::ATK) - _Player->GetDebuffParam(CharacterParameter::Param::ATK));
			_ParameterRenderList[static_cast<int>(CIShowStatus::MAT)]->SetParam("MAT", "UI/S_Buff02.png", _Player->GetParam(CharacterParameter::Param::MAT), fbText::TextAnchorE::MiddleRight, _Player->GetBuffParam(CharacterParameter::Param::MAT) - _Player->GetDebuffParam(CharacterParameter::Param::MAT));
			_ParameterRenderList[static_cast<int>(CIShowStatus::DEF)]->SetParam("DEF", "UI/S_Buff03.png", _Player->GetParam(CharacterParameter::Param::DEF), fbText::TextAnchorE::MiddleRight, _Player->GetBuffParam(CharacterParameter::Param::DEF) - _Player->GetDebuffParam(CharacterParameter::Param::DEF));
			_ParameterRenderList[static_cast<int>(CIShowStatus::MDE)]->SetParam("MDE", "UI/S_Buff03.png", _Player->GetParam(CharacterParameter::Param::MDE), fbText::TextAnchorE::MiddleRight, _Player->GetBuffParam(CharacterParameter::Param::MDE) - _Player->GetDebuffParam(CharacterParameter::Param::MDE));
			_ParameterRenderList[static_cast<int>(CIShowStatus::DEX)]->SetParam("DEX", "UI/S_Buff02.png", _Player->GetParam(CharacterParameter::Param::DEX), fbText::TextAnchorE::MiddleRight, _Player->GetBuffParam(CharacterParameter::Param::DEX) - _Player->GetDebuffParam(CharacterParameter::Param::DEX));
			_ParameterRenderList[static_cast<int>(CIShowStatus::MONEY)]->SetParam("MONEY", "UI/coins.png", INSTANCE(Inventory)->GetPlayerMoney());

			if (playerLevel != _playerLevel) {
				// レベル変動。
				_playerLevel = playerLevel;
				_ExpBar->Reset(static_cast<float>(_Player->GetNextLevelExp()), static_cast<float>(_Player->GetExp()), false);
				_HpBar->Reset(static_cast<float>(_Player->GetMaxHP()), static_cast<float>(_Player->GetParam(CharacterParameter::HP)), false);
				_MpBar->Reset(static_cast<float>(_Player->GetMaxMP()), static_cast<float>(_Player->GetParam(CharacterParameter::MP)), false);
			}
			else {
				_ExpBar->SetValue(static_cast<float>(_Player->GetExp()));
				_HpBar->SetValue(static_cast<float>(_Player->GetParam(CharacterParameter::HP)));
				_MpBar->SetValue(static_cast<float>(_Player->GetParam(CharacterParameter::MP)));
			}
			break;
		}
		case Item::ItemCodeE::Armor:
		{	
			HoldArmor* armor = _Player->GetEquipment()->armor;
			HoldArmor* newArmor = (HoldArmor*)_Item2DList[_NowSelectItem]->GetItemData();

			_ParameterRenderList[(int)AIShowStatus::RANK]->SetParam("RANK", "UI/S_Buff02.png", 0);

			int defParam = 0;
			int defnewParam = 0;
			int mdeParam = 0;
			int mdenewParam = 0;

			HoldEquipment::Rank rank = HoldEquipment::Rank::None;
			HoldEquipment::Rank newRank = HoldEquipment::Rank::None;

			if (armor)
			{
				defParam = armor->GetDef();
				mdeParam = armor->GetMagicDef();
				rank = armor->GetRank();
			}
			if (newArmor)
			{
				defnewParam = newArmor->GetDef();
				mdenewParam = newArmor->GetMagicDef();
				newRank = newArmor->GetRank();
			}
			_ParameterRenderList[(int)AIShowStatus::RANK]->SetParamRank("RANK", "UI/S_Buff02.png", rank, newRank);
			_ParameterRenderList[(int)AIShowStatus::DEF]->SetParamEquip("DEF", "UI/S_Buff02.png", 
				_Player->GetParam(CharacterParameter::Param::DEF), defParam, defnewParam);
			_ParameterRenderList[(int)AIShowStatus::MDE]->SetParamEquip("MDE", "UI/S_Buff02.png",
				_Player->GetParam(CharacterParameter::Param::MDE), mdeParam, mdenewParam);
			_ParameterRenderList[(int)AIShowStatus::CRT]->SetParamEquip("CRT", "UI/S_Buff02.png",
				_Player->GetParam(CharacterParameter::Param::CRT), 0, 0);
			_ParameterRenderList[(int)AIShowStatus::DEX]->SetParamEquip("DEX", "UI/S_Buff02.png",
				_Player->GetParam(CharacterParameter::Param::DEX), 0, 0);
			break;
		}
		case Item::ItemCodeE::Weapon:
		{	
			HoldWeapon* weapon = _Player->GetEquipment()->weapon;
			HoldWeapon* newWeapon = (HoldWeapon*)_Item2DList[_NowSelectItem]->GetItemData();

			int atkParam = 0;
			int atknewParam = 0;
			int matParam = 0;
			int matnewParam = 0;

			HoldEquipment::Rank rank = HoldEquipment::Rank::None;
			HoldEquipment::Rank newRank = HoldEquipment::Rank::None;

			if (weapon)
			{
				atkParam = weapon->GetAtk();
				matParam = weapon->GetMagicAtk();
				rank = weapon->GetRank();
			}
			if (newWeapon)
			{
				atknewParam = newWeapon->GetAtk();
				matnewParam = newWeapon->GetMagicAtk();
				newRank = newWeapon->GetRank();
			}
			_ParameterRenderList[(int)WIShowStatus::RANK]->SetParamRank("RANK", "UI/S_Buff02.png", rank, newRank);
			_ParameterRenderList[(int)WIShowStatus::ATK]->SetParamEquip("ATK", "UI/S_Buff02.png",
				_Player->GetParam(CharacterParameter::Param::ATK), atkParam, atknewParam);
			_ParameterRenderList[(int)WIShowStatus::MAT]->SetParamEquip("MAT", "UI/S_Buff02.png",
				_Player->GetParam(CharacterParameter::Param::MAT), matParam, matnewParam);
			_ParameterRenderList[(int)WIShowStatus::CRT]->SetParamEquip("CRT", "UI/S_Buff02.png",
				_Player->GetParam(CharacterParameter::Param::CRT), 0, 0);
			_ParameterRenderList[(int)WIShowStatus::DEX]->SetParamEquip("DEX", "UI/S_Buff02.png", 
				_Player->GetParam(CharacterParameter::Param::DEX), 0, 0);
			break;
		}
	}

}