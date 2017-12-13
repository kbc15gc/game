/**
* アイテムウィンドウクラスの実装.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"GameObject\Inventory\Inventory.h"
#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include"GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include "GameObject\StatusWindow\StatusWindow.h"
#include "GameObject\TextImage\AttentionTextOnly.h"

const char* ItemWindow::IconTextureNameList[static_cast<int>(IconIndex::MAX)] = {
	"UI/gem.png",
	"UI/hp.png",
	//"UI/mp.png",
	"sword.png",
	//"magic.png",
	"armor.png",
	"cloaks.PNG",
	"UI/S_Light01.png",
	"UI/S_Light01.png",	// 暫定。
	"UI/gem.png",	// 暫定。
};

/**
* 初期化.
*/
void ItemWindow::Awake()
{
	_ItemBackWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", StatusWindow::WindowBackPriorty + 1);
	_ItemBackWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	_ItemBackWindow->SetSize(Vector2(495.0f, 580.0f));
	_ItemBackWindow->SetPivot(0.0f, 0.5f);
	_ItemBackWindow->transform->SetParent(transform);
	_ItemBackWindow->transform->SetLocalPosition(Vector3(0.0f, 47.0f, 0.0f));

	_WindowName = INSTANCE(GameObjectManager)->AddNew<TextObject>("WindowName", StatusWindow::WindowBackPriorty + 2);
	_WindowName->Initialize(L"", 30.0f);
	_WindowName->SetAnchor(fbText::TextAnchorE::UpperCenter);
	_WindowName->transform->SetParent(transform);
	_WindowName->transform->SetLocalPosition(Vector3(250.0f, -235.0f, 0.0f));

	for (int i = 0; i < ItemCellSize; i++)
	{
		Item2D* item2D = INSTANCE(GameObjectManager)->AddNew<Item2D>("Item2D", StatusWindow::WindowBackPriorty + 3);
		item2D->transform->SetParent(_ItemBackWindow->transform);
		item2D->transform->SetLocalPosition(Vector3(270.0f, -190.0f + (i * 52.0f), 0.0f));
		_Item2DList.push_back(item2D);
	}

	//_SelectCursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", StatusWindow::WindowBackPriorty + 3);
	//_SelectCursor->SetTexture(LOADTEXTURE("cursor.png"));
	//_SelectCursor->SetSize(Vector2(50.0f, 50.0f));
	Quaternion rot = Quaternion::Identity;
	//rot.SetEuler(Vector3(0.0f, 0.0f, -90.0f));
	//_SelectCursor->transform->SetLocalRotation(rot);
	//_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
	//_SelectCursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));

	_Cursor = INSTANCE(GameObjectManager)->AddNew<Cursor>("SelectCursor", StatusWindow::WindowBackPriorty + 3);
	_Cursor->CreateCursor("cursor.png", _Item2DList[_NowSelectItem]->transform, Vector2(-230.0f, 0.0f), Vector2(50.0f, 50.0f),0.0f , ItemCellSize, ItemCellSize, true, -90.0f);

	_UpArrow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("UpArrow", StatusWindow::WindowBackPriorty + 3);
	_UpArrow->SetTexture(LOADTEXTURE("cursor.png"));
	_UpArrow->SetSize(Vector2(40.0f, 40.0f));
	rot = Quaternion::Identity;
	rot.SetEuler(Vector3(0.0f, 0.0f, -180.0f));
	_UpArrow->transform->SetLocalRotation(rot);
	_UpArrow->transform->SetParent(_Item2DList[0]->transform);
	_UpArrow->transform->SetLocalPosition(Vector3(0.0f, -40.0f, 0.0f));
	_UpArrow->SetActive(false,false);

	_DownArrow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DownArrow", StatusWindow::WindowBackPriorty + 3);
	_DownArrow->SetTexture(LOADTEXTURE("cursor.png"));
	_DownArrow->SetSize(Vector2(40.0f, 40.0f));
	_DownArrow->transform->SetParent(_Item2DList[ItemCellSize - 1]->transform);
	_DownArrow->transform->SetLocalPosition(Vector3(0.0f, 40.0f, 0.0f));
	_DownArrow->SetActive(false, false);

	_EIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("EIconImage", StatusWindow::WindowBackPriorty + 3);
	_EIconImage->SetTexture(LOADTEXTURE("UI/E.png"));
	_EIconImage->SetSize(Vector2(30, 30));
	_EIconImage->SetActive(false, true);

	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}


/**
* 初期化.
*/
void ItemWindow::Init(Item::ItemCodeE code)
{
	_ItemCode = code;

	_Dialog = INSTANCE(GameObjectManager)->AddNew<Dialog>("", 9);
	_Dialog->Init(code);

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

	//Vector3 pos[4] =
	//{
	//	Vector3(-250.0f,140.0f,0.0f),	//上
	//	Vector3(-250.0f,250.0f,0.0f),	//下
	//	Vector3(-220.0f,195.0f,0.0f),	//右
	//	Vector3(-280.0f,195.0f,0.0f),	//左
	//};
	//for (int i = 0; i < 4; i++)
	//{
	//	HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
	//	holdItem->transform->SetParent(transform);
	//	holdItem->transform->SetLocalPosition(pos[i]);
	//	holdItem->Init((i % 2 == 1));
	//	_HoldItem2DList.push_back(holdItem);
	//}
}

/**
* 武器の初期化.
*/
void ItemWindow::WeaponInit()
{
	_WindowName->SetText(L"武器一覧");

	_CreateWIShowStatus();

	//HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
	//holdItem->transform->SetParent(transform);
	//holdItem->transform->SetLocalPosition(Vector3(-400.0f, 250.0f, 0.0f));
	//holdItem->Init();
	//_HoldItem2DList.push_back(holdItem);
}

/**
* 防具の初期化.
*/
void ItemWindow::ArmorInit()
{
	_WindowName->SetText(L"防具一覧");

	_CreateAIShowStatus();

	//HoldItem2D* holdItem = INSTANCE(GameObjectManager)->AddNew<HoldItem2D>("", 9);
	//holdItem->transform->SetParent(transform);
	//holdItem->transform->SetLocalPosition(Vector3(-400.0f, 250.0f, 0.0f));
	//holdItem->Init();
	//_HoldItem2DList.push_back(holdItem);
}

/**
* 更新.
*/
void ItemWindow::LateUpdate()
{
	Input();

	_EIconImage->SetActive(false, true);
	/*for (auto it : _HoldItem2DList)
	{
		it->SetHoldItem(nullptr);
	}*/

	auto& itemList = INSTANCE(Inventory)->GetInventoryList(_ItemCode);
	for (int i = 0; i < ItemCellSize; i++)
	{
		if (itemList.size() > i + _StartLoadCount && itemList[i + _StartLoadCount] != nullptr)
		{
			_Item2DList[i]->SetItemData(itemList[i + _StartLoadCount]);

			if (_ItemCode != Item::ItemCodeE::Item)
			{
				HoldEquipment* item = (HoldEquipment*)itemList[i + _StartLoadCount];
				if (item->GetIsEquip())
				{
					_EIconImage->SetActive(true, true);
					_EIconImage->transform->SetLocalPosition(_Item2DList[i]->transform->GetPosition() + Vector3(180.0f, 0.0f, 0.0f));
					//_HoldItem2DList[0]->SetHoldItem(_Item2DList[i]->GetItemData());
				}
			}
		}
		else
		{
			_Item2DList[i]->SetItemData(nullptr);
		}
	}

	ArrowUpdate();

	_ConfigParamRender();
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

	_Cursor->SetMax(itemCount);
	if (itemCount <= 0)
	{
		_Cursor->SetActive(false, false);
	}
	else
	{
		_Cursor->SetActive(true, true);

		static float ChangeTime = 0.5f;
		static float LocalTime = 0.0f;

		if (!_Dialog->GetActive())
		{
			Vector2 LStick = XboxInput(0)->GetAnalog(AnalogE::L_STICK);
			LStick /= 32767.0f;
			if (LStick.y >= 0.2f || XboxInput(0)->IsPressButton(XINPUT_GAMEPAD_DPAD_UP))
			{
				if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_UP))
				{
					Cursor::CursorIndex index = _Cursor->PrevMove(1);
					_NowSelectItem = index.rangeIndex;
					_StartLoadCount = index.offset;
					_Cursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
					_Cursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));
				}
				LocalTime += Time::DeltaTime();
				if (LocalTime >= ChangeTime)
				{
					LocalTime = 0.0f;
					ChangeTime = 0.01f;
					Cursor::CursorIndex index = _Cursor->PrevMove(1);
					_NowSelectItem = index.rangeIndex;
					_StartLoadCount = index.offset;
					_Cursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
					_Cursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));
				}
			}
			else if (LStick.y <= -0.2f || XboxInput(0)->IsPressButton(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_DOWN))
				{
					Cursor::CursorIndex index = _Cursor->NextMove(1);
					_NowSelectItem = index.rangeIndex;
					_StartLoadCount = index.offset;
					_Cursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
					_Cursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));
				}
	
				LocalTime += Time::DeltaTime();
				if (LocalTime >= ChangeTime)
				{
					LocalTime = 0.0f;
					ChangeTime = 0.01f;
					Cursor::CursorIndex index = _Cursor->NextMove(1);
					_NowSelectItem = index.rangeIndex;
					_StartLoadCount = index.offset;
					_Cursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
					_Cursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));
				}
			}
			else
			{
				LocalTime = 0.0f;
				ChangeTime = 0.5f;
			}

			if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
			{
				_Dialog->Enable(_Item2DList[_NowSelectItem]);
			}

		}
		else
		{
			Dialog::DialogCommand commang = _Dialog->InputUpdate();

			bool isPack = false;

			switch (commang)
			{
				case Dialog::None:
					//入力なし.
					break;
				case Dialog::Equip:
				{
					//装備もしくは外す.
					_Player->SetEquipment(_Item2DList[_NowSelectItem]->GetItemData());
					SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
					se->Init("Asset/Sound/UI/Equipment.wav");
					se->SetDelete(true);
					se->Play(false);
					break;
				}
				case Dialog::Dump:
				{
					//捨てる.
					INSTANCE(Inventory)->SubHoldNum(_Item2DList[_NowSelectItem]->GetItemData(), 1);
					SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
					se->Init("Asset/Sound/UI/tableware_crash1.wav");
					se->SetDelete(true);
					se->Play(false);
					isPack = true;
					break;
				}
				case Dialog::UseItem:
					//アイテム使用.
				{
					ConsumptionItem* item = (ConsumptionItem*)_Item2DList[_NowSelectItem]->GetItemData();
					//アイテム使用が失敗したかどうかをチェック。
					if (item->UseItem() == false) {
						
					}
					SoundSource* se = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
					se->Init("Asset/Sound/UI/drinking1.wav");
					se->SetDelete(true);
					se->Play(false);
					INSTANCE(Inventory)->SubHoldNum(item, 1);
					isPack = true;
					break;
				}
			}

			//リストが詰められた可能性があるため表示を更新.
			if (isPack)
			{

				itemCount = 0;
				for (auto& item : itemList)
				{
					if (item != nullptr)
					{
						itemCount += 1;
					}
				}
				_Cursor->SetMax(itemCount);	// 要素数を更新。
				if (_StartLoadCount > 0 && ItemCellSize + _StartLoadCount > itemCount)
				{
					//表示位置を一個さげる.
					_StartLoadCount = max(0, _StartLoadCount - 1);
				}
				else if (_NowSelectItem >= itemCount)
				{
					//選択位置を一個下げる.
					int index = _Cursor->PrevMove(1).rangeIndex;
					_NowSelectItem = max(0, index);
				}

				if (itemCount <= 0)
				{
					_Cursor->transform->SetParent(nullptr);
					_Cursor->SetActive(false, true);
				}
				else
				{
					_Cursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
					_Cursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));
				}
			}
		}


	}
}

void ItemWindow::ArrowUpdate()
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
	if (itemCount > ItemCellSize) {
		// 所持枠を埋めているアイテムが表示限界枠より多い。

		if (_StartLoadCount > 0) {
			// 一番上に表示されているアイテムがアイテムリストの先頭ではない。

			_UpArrow->SetActive(true, true);
		}
		else {
			_UpArrow->SetActive(false, false);
		}

		if (_StartLoadCount + ItemCellSize < itemCount) {
			// 一番下に表示されているアイテムがアイテムリストの終端ではない。

			_DownArrow->SetActive(true, true);
		}
		else {
			_DownArrow->SetActive(false, false);
		}
	}
	else {
		// 所持枠を埋めているアイテムが表示限界枠におさまっている。

		_UpArrow->SetActive(false, false);
		_DownArrow->SetActive(false, false);
	}
}

void ItemWindow::_CreateCIShowStatus()
{
	CIShowStatus ParamCount = CIShowStatus::MAX;
	Transform* work = transform;
	Vector3 pos = Vector3(-280.0f, -270.0f, 0.0f);
	for (int i = 0; i < static_cast<int>(ParamCount); i++)
	{
		//if (i == CharacterParameter::Param::MAT || i == CharacterParameter::Param::MP) {
		//	// 魔法攻撃と魔法ポイントは無視。
		//	continue;
		//}
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(work);
		pr->transform->SetLocalPosition(pos + Vector3(0.0f,40.0f, 0.0f));
		_ParameterRenderList.push_back(pr);
		work = pr->transform;
		pos = Vector3::zero;
	}
	// レベルのパラメータは真横に表示。
	_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->SetNameTextPos(_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->GetNameTextPos() + Vector3(0.0f,10.0f,0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->GetParamTextPos() + Vector3(-320, -7.0f, 0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->GetIconObject()->transform->SetLocalPosition(_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->GetIconObject()->transform->GetLocalPosition() + Vector3(0.0f, 5.0f, 0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->transform->SetLocalPosition(_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->transform->GetLocalPosition() + Vector3(15.0f,25.0f,0.0f));
	//_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->transform->SetLocalPosition(_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->transform->GetLocalPosition() + Vector3(0.0f, -5.0f, 0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->GetParamTextPos() + Vector3(-290, -4.5f, 0.0f));
	//_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->GetParamTextPos() + Vector3(-290, -4.5f, 0.0f));
	_ParameterRenderList[static_cast<int>(CIShowStatus::ATK)]->transform->SetLocalPosition(_ParameterRenderList[static_cast<int>(CIShowStatus::ATK)]->transform->GetLocalPosition() + Vector3(0.0f, 15.0f, 0.0f));

	_ExpBar = AddComponent<ParameterBar>();
	vector<BarColor> barColor;
	barColor.push_back(BarColor::Yellow);
	_ExpBar->Create(barColor, static_cast<float>(_Player->GetNextLevelExp()), static_cast<float>(_Player->GetExp()), false, false, _ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->transform, Vector3(25.0f, 46.0f, 0.0f), Vector2(1.16f,0.35f), 8,true);

	_HpBar = AddComponent<ParameterBar>();
	barColor.clear();
	barColor.push_back(BarColor::Green);
	_HpBar->Create(barColor, static_cast<float>(_Player->GetMaxHP()), static_cast<float>(_Player->GetParam(CharacterParameter::Param::HP)), false, false, _ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->transform, Vector3(50.0f,18.0f, 0.0f), Vector2(1.0f,0.7f), 8, false);

	//_MpBar = AddComponent<ParameterBar>();
	//barColor.clear();
	//barColor.push_back(BarColor::Blue);
	//_MpBar->Create(barColor, static_cast<float>(_Player->GetMaxMP()), static_cast<float>(_Player->GetParam(CharacterParameter::Param::MP)), false, false, _ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->transform, Vector3(50.0f, 18.0f, 0.0f), Vector2(1.0f,0.7f), 8, false);

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

	_ParameterRenderList[static_cast<int>(WIShowStatus::RANK)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(WIShowStatus::RANK)]->GetParamTextPos() + Vector3(-40, -7.0f, 0.0f));


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

	_ParameterRenderList[static_cast<int>(AIShowStatus::RANK)]->SetParamTextPos(_ParameterRenderList[static_cast<int>(AIShowStatus::RANK)]->GetParamTextPos() + Vector3(-40, -7.0f, 0.0f));

	_ConfigParamRender();
}

void ItemWindow::_ConfigParamRender() 
{

	switch (_ItemCode)
	{
		case Item::ItemCodeE::Item:
		{
			int playerLevel = _Player->GetParam(CharacterParameter::Param::LV);
			_ParameterRenderList[static_cast<int>(CIShowStatus::LV)]->SetParam("LV", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::LV)]), playerLevel, fbText::TextAnchorE::UpperLeft, 40.0f, Vector2(40.0f, 40.0f),50.0f);
			_ParameterRenderList[static_cast<int>(CIShowStatus::HP)]->SetParamMax("HP", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::HP)]), _Player->GetParam(CharacterParameter::Param::HP), _Player->GetMaxHP(),fbText::TextAnchorE::UpperLeft);
			//_ParameterRenderList[static_cast<int>(CIShowStatus::MP)]->SetParamMax("MP", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::MP)]), _Player->GetParam(CharacterParameter::Param::MP), _Player->GetMaxMP(), fbText::TextAnchorE::UpperLeft);
			_ParameterRenderList[static_cast<int>(CIShowStatus::ATK)]->SetParamBuff("ATK", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::ATK)]), _Player->GetParam(CharacterParameter::Param::ATK), _Player->GetBuffParam(CharacterParameter::Param::ATK) - _Player->GetDebuffParam(CharacterParameter::Param::ATK));
			//_ParameterRenderList[static_cast<int>(CIShowStatus::MAT)]->SetParamBuff("MAT", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::MAT)]), _Player->GetParam(CharacterParameter::Param::MAT), _Player->GetBuffParam(CharacterParameter::Param::MAT) - _Player->GetDebuffParam(CharacterParameter::Param::MAT));
			_ParameterRenderList[static_cast<int>(CIShowStatus::DEF)]->SetParamBuff("DEF", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::DEF)]), _Player->GetParam(CharacterParameter::Param::DEF), _Player->GetBuffParam(CharacterParameter::Param::DEF) - _Player->GetDebuffParam(CharacterParameter::Param::DEF));
			_ParameterRenderList[static_cast<int>(CIShowStatus::MDE)]->SetParamBuff("MDE", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::MDE)]), _Player->GetParam(CharacterParameter::Param::MDE), _Player->GetBuffParam(CharacterParameter::Param::MDE) - _Player->GetDebuffParam(CharacterParameter::Param::MDE));
			_ParameterRenderList[static_cast<int>(CIShowStatus::DEX)]->SetParamBuff("DEX", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::DEX)]), _Player->GetParam(CharacterParameter::Param::DEX), _Player->GetBuffParam(CharacterParameter::Param::DEX) - _Player->GetDebuffParam(CharacterParameter::Param::DEX));

			if (playerLevel != _playerLevel) {
				// レベル変動。
				_playerLevel = playerLevel;
				_ExpBar->Reset(static_cast<float>(_Player->GetNextLevelExp()), static_cast<float>(_Player->GetExp()), false);
				_HpBar->Reset(static_cast<float>(_Player->GetMaxHP()), static_cast<float>(_Player->GetParam(CharacterParameter::HP)), false);
				//_MpBar->Reset(static_cast<float>(_Player->GetMaxMP()), static_cast<float>(_Player->GetParam(CharacterParameter::MP)), false);
			}
			else {
				_ExpBar->SetValue(static_cast<float>(_Player->GetExp()));
				_HpBar->SetValue(static_cast<float>(_Player->GetParam(CharacterParameter::HP)));
				//_MpBar->SetValue(static_cast<float>(_Player->GetParam(CharacterParameter::MP)));
			}
			break;
		}
		case Item::ItemCodeE::Armor:
		{	
			HoldArmor* armor = _Player->GetEquipment()->armor;

			HoldArmor* newArmor = (HoldArmor*)_Item2DList[_NowSelectItem]->GetItemData();

			//_ParameterRenderList[(int)AIShowStatus::RANK]->SetParam("RANK", "UI/S_Buff02.png", 0);

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
			_ParameterRenderList[(int)AIShowStatus::DEF]->SetParamEquip("DEF", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::DEF)]),
				_Player->GetParam(CharacterParameter::Param::DEF), defParam, defnewParam);
			_ParameterRenderList[(int)AIShowStatus::MDE]->SetParamEquip("MDE", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::MDE)]),
				_Player->GetParam(CharacterParameter::Param::MDE), mdeParam, mdenewParam);
			_ParameterRenderList[(int)AIShowStatus::CRT]->SetParamEquip("CRT", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::CRT)]),
				_Player->GetParam(CharacterParameter::Param::CRT), 0, 0);
			_ParameterRenderList[(int)AIShowStatus::DEX]->SetParamEquip("DEX", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::DEX)]),
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
			int crtParam = 0;
			int crtnewParam = 0;
			int dexParam = 0;
			int dexnewParam = 0;

			HoldEquipment::Rank rank = HoldEquipment::Rank::None;
			HoldEquipment::Rank newRank = HoldEquipment::Rank::None;

			if (weapon)
			{
				atkParam = weapon->GetAtk();
				matParam = weapon->GetMagicAtk();
				crtParam = weapon->GetCrt();
				dexParam = weapon->GetDex();
				rank = weapon->GetRank();
			}
			if (newWeapon)
			{
				atknewParam = newWeapon->GetAtk();
				matnewParam = newWeapon->GetMagicAtk();
				crtnewParam = newWeapon->GetCrt();
				dexnewParam = newWeapon->GetDex();
				newRank = newWeapon->GetRank();
			}
			_ParameterRenderList[(int)WIShowStatus::RANK]->SetParamRank("RANK", "UI/S_Buff02.png", rank, newRank);
			_ParameterRenderList[(int)WIShowStatus::ATK]->SetParamEquip("ATK", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::ATK)]),
				_Player->GetParam(CharacterParameter::Param::ATK), atkParam, atknewParam);
			//_ParameterRenderList[(int)WIShowStatus::MAT]->SetParamEquip("MAT", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::MAT)]),
			//	_Player->GetParam(CharacterParameter::Param::MAT), matParam, matnewParam);
			_ParameterRenderList[(int)WIShowStatus::CRT]->SetParamEquip("CRT", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::CRT)]),
				_Player->GetParam(CharacterParameter::Param::CRT), crtParam, crtnewParam);
			_ParameterRenderList[(int)WIShowStatus::DEX]->SetParamEquip("DEX", const_cast<char*>(IconTextureNameList[static_cast<int>(IconIndex::DEX)]),
				_Player->GetParam(CharacterParameter::Param::DEX), dexParam, dexnewParam);
			break;
		}
	}

}