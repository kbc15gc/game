/**
* アイテム表示画面クラスの実装.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"GameObject\Inventory\Inventory.h"
#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"

/**
* 初期化.
*/
void ItemWindow::Awake()
{
	//ウィンドウ名の初期化.
	_WindowName = INSTANCE(GameObjectManager)->AddNew<TextObject>("WindowName", 9);
	_WindowName->Initialize(L"消費アイテム", 30.0f);
	_WindowName->SetFormat(fbText::TextFormatE::CENTER);
	_WindowName->transform->SetParent(transform);
	_WindowName->transform->SetLocalPosition(Vector3(250.0f, -280.0f, 0.0f));

	for (int i = 0; i < 8; i++)
	{
		Item2D* item = INSTANCE(GameObjectManager)->AddNew<Item2D>("Item2D", 9);
		item->transform->SetParent(transform);
		item->transform->SetLocalPosition(Vector3(250.0f, -220.0f + (i * 35.0f), 0.0f));
		_Item2DList.push_back(item);
	}
}

/**
* 更新.
*/
void ItemWindow::Update()
{
	auto& itemList = INSTANCE(Inventory)->GetInventoryList(_ItemCode);

	for (int i = 0; i < 8; i++)
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
