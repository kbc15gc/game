/**
* ダイアログクラスの実装.
*/
#include"stdafx.h"
#include"Dialog.h"

#include"GameObject\ItemManager\HoldItem\ConsumptionItem.h"
#include"GameObject\ItemManager\HoldItem\HoldEquipment.h"

namespace
{
	wchar_t* ItemCommandName[]=
	{
		L"使う",
		L"捨てる",
	};

	wchar_t* EquipCommandName[] =
	{
		L"装備する",
		L"捨てる",
	};
}

/**
* 初期化.
*/
void Dialog::Awake()
{
	_BackWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("", 9);
	_BackWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	_BackWindow->transform->SetParent(transform);
	_BackWindow->SetPivot(0.0f, 0.0f);
	_BackWindow->SetSize(Vector2(200.0f, 150.0f));
	
	_Cursor = INSTANCE(GameObjectManager)->AddNew<Cursor>("SelectCursor", 9);

}

/**
* 初期化.
*/
void Dialog::Init(Item::ItemCodeE code)
{
	Vector3 pos = Vector3(40.0f, 20.0f, 0.0f);

	_ItemCode = code;

	for (int i = 0; i < 2; i++)
	{
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("", 9);
		if (Item::ItemCodeE::Item == _ItemCode)
		{
			text->Initialize(ItemCommandName[i], 30.0f);
		}
		else
		{
			text->Initialize(EquipCommandName[i], 30.0f);

		}
		text->transform->SetParent(_BackWindow->transform);
		text->transform->SetLocalPosition(pos);
		text->SetAnchor(fbText::TextAnchorE::UpperLeft);
		pos.y += 35.0f;
		_CommandList.push_back(text);
	}
	_Cursor->CreateCursor("cursor.png", _CommandList[0]->transform, Vector2(-20.0f, 15.0f), Vector2(50.0f, 50.0f), 0.0f, 2, 2, true, -90.0f);
	_BackWindow->SetSize(Vector2(200.0f, 100.0f));

	SetActive(false, true);
}

/**
* 更新.
*/
Dialog::DialogCommand Dialog::InputUpdate()
{
	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKU) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_UP))
	{
		_NowSelect = _Cursor->PrevMove(1).rangeIndex;
		_Cursor->transform->SetParent(_CommandList[_NowSelect]->transform);
		_Cursor->transform->SetLocalPosition(Vector3(-20.0f, 15.0f, 0.0f));
	}
	if (XboxInput(0)->IsPushAnalog(AnalogE::L_STICKD) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		_NowSelect = _Cursor->NextMove(1).rangeIndex;
		_Cursor->transform->SetParent(_CommandList[_NowSelect]->transform);
		_Cursor->transform->SetLocalPosition(Vector3(-20.0f, 15.0f, 0.0f));
	}

	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		switch (_ItemCode)
		{
			case Item::ItemCodeE::Item:
				switch (_NowSelect)
				{
					case 0:
						//アイテム使用.
						SetActive(false, true);
						_Cursor->SetActive(false, true);
						return DialogCommand::UseItem;
					case 1:
						//捨てる.
						SetActive(false, true);
						_Cursor->SetActive(false, true);
						return DialogCommand::Dump;
				}
			case Item::ItemCodeE::Armor:
			case Item::ItemCodeE::Weapon:
				switch (_NowSelect)
				{
					case 0:
						//装備.
						SetActive(false, true);
						_Cursor->SetActive(false, true);
						return DialogCommand::Equip;
					case 1:
						//捨てる.
						SetActive(false, true);
						_Cursor->SetActive(false, true);
						return DialogCommand::Dump;
				}
		}
	}

	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		SetActive(false, true);
		_Cursor->SetActive(false, true);
		return DialogCommand::None;
	}

	return DialogCommand::None;
}

/**
* 有効にする.
*/
void Dialog::Enable(Item2D * item)
{
	SetActive(true, true);
	transform->SetLocalPosition(item->transform->GetPosition() + Vector3(100.0f, 0.0f, 0.0f));
	_Cursor->SetActive(true, true);

	if (_ItemCode != Item::ItemCodeE::Item)
	{
		HoldEquipment* eItem = (HoldEquipment*)item->GetItemData();

		if (eItem->GetIsEquip())
		{
			_CommandList[0]->SetText(L"外す");
			_CommandList[1]->SetActive(false);
			_Cursor->SetMax(1);
			_BackWindow->SetSize(Vector2(200.0f, 70.0f));
		}
		else
		{
			_CommandList[0]->SetText(L"装備する");
			_CommandList[1]->SetActive(true);
			_Cursor->SetMax(2);
			_BackWindow->SetSize(Vector2(200.0f, 100.0f));
		}

		_Cursor->SetIndex(0, 0, 0);
		_NowSelect = 0;
		_Cursor->transform->SetParent(_CommandList[_NowSelect]->transform);
		_Cursor->transform->SetLocalPosition(Vector3(-20.0f, 15.0f, 0.0f));
	}
}
