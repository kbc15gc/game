/**
* �A�C�e���\����ʃN���X�̎���.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"GameObject\Inventory\Inventory.h"
#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"

/**
* ������.
*/
void ItemWindow::Awake()
{
	//�E�B���h�E���̏�����.
	_WindowName = INSTANCE(GameObjectManager)->AddNew<TextObject>("WindowName", 9);
	_WindowName->Initialize(L"����A�C�e��", 30.0f);
	_WindowName->SetFormat(fbText::TextFormatE::CENTER);
	_WindowName->transform->SetParent(transform);
	_WindowName->transform->SetLocalPosition(Vector3(250.0f, -280.0f, 0.0f));

	//�A�C�e���Z���̏�����.
	for (int i = 0; i < ItemCellSize; i++)
	{
		Item2D* item = INSTANCE(GameObjectManager)->AddNew<Item2D>("Item2D", 9);
		item->transform->SetParent(transform);
		item->transform->SetLocalPosition(Vector3(270.0f, -220.0f + (i * 52.0f), 0.0f));
		_Item2DList.push_back(item);
	}

	//�Z���N�g�J�[�\��. 
	_SelectCursor = INSTANCE(GameObjectManager)->AddNew<ImageObject>("SelectCursor", 9);
	_SelectCursor->SetTexture(LOADTEXTURE("cursor.png"));
	_SelectCursor->SetSize(Vector2(50.0f, 50.0f));
	Quaternion rot = Quaternion::Identity;
	rot.SetEuler(Vector3(0.0f, 0.0f, -90.0f));
	_SelectCursor->transform->SetLocalRotation(rot);
	_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
	_SelectCursor->transform->SetLocalPosition(Vector3(-230.0f, 0.0f, 0.0f));
}

/**
* �X�V.
*/
void ItemWindow::Update()
{
	Input();

	auto& itemList = INSTANCE(Inventory)->GetInventoryList(_ItemCode);
	for (int i = 0; i < ItemCellSize; i++)
	{
		if (itemList.size() > i && itemList[i] != nullptr)
		{
			_Item2DList[i]->SetActive(true, true);
			_Item2DList[i]->SetItemData(itemList[i]);
		}
		else
		{
			_Item2DList[i]->SetActive(false, true);
		}
	}
}

/**
* ����.
*/
void ItemWindow::Input()
{
	static float ChangeTime = 0.5f;
	static float LocalTime = 0.0f;
	//���X�e�B�b�N�̏��.
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
			_NowSelectItem = min(ItemCellSize - 1, _NowSelectItem + 1);
			_SelectCursor->transform->SetParent(_Item2DList[_NowSelectItem]->transform);
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			_NowSelectItem = min(ItemCellSize - 1, _NowSelectItem + 1);
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
}
